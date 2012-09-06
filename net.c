//
//  net.c
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>

#include "uapn.h"

static int setup_ssl(uapn_connection *connection)
{
  SSL_library_init();
  SSL_load_error_strings();
  connection->ssl.method = SSLv3_method();
  if(!(connection->ssl.ctx = SSL_CTX_new(connection->ssl.method)))
	return error(UAPN_ERROR_SSL_CONTEXT);
  if (SSL_CTX_use_certificate_file(connection->ssl.ctx, 
								   connection->cert_path, 
								   SSL_FILETYPE_PEM) <= 0)
	return error(UAPN_ERROR_INVALID_CERT);
  if (SSL_CTX_use_PrivateKey_file(connection->ssl.ctx, 
								  connection->key_path, SSL_FILETYPE_PEM) <= 0)
	return error(UAPN_ERROR_INVALID_KEY);
  if (!SSL_CTX_check_private_key(connection->ssl.ctx))
	return error(UAPN_ERROR_CERT_KEY_MISMATCH);
  if((connection->ssl.sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	return error(UAPN_ERROR_SOCKET);

  return 0;
}

static int make_socket(uapn_connection *connection, int port, char* url)
{
  memset (&connection->ssl.server_addr, 0, sizeof(connection->ssl.server_addr));
  connection->ssl.server_addr.sin_family = AF_INET;
  connection->ssl.server_addr.sin_port = htons(port);
  connection->ssl.host_info = gethostbyname(url);
  
  if(!connection->ssl.host_info)
	return error(UAPN_ERROR_HOSTNAME);

  struct in_addr *address = (struct in_addr*)connection->ssl.host_info->h_addr_list[0];
  connection->ssl.server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*address));
  
  if(connect(connection->ssl.sock,
			 (struct sockaddr*) &connection->ssl.server_addr, 
			 sizeof(connection->ssl.server_addr)) == -1)
	return error(UAPN_ERROR_CONNECT);
  if(!(connection->ssl.ssl = SSL_new(connection->ssl.ctx)))
	return error(UAPN_ERROR_SSL_SOCKET);
  SSL_set_fd(connection->ssl.ssl, connection->ssl.sock);
  if(SSL_connect(connection->ssl.ssl) <= -1)
	return error(UAPN_ERROR_SSL_CONNECT);
  
  return 0;
}

static int connect_to_service(uapn_connection *connection, char *url, int port)
{
  int err = 0;

  if(!(err = setup_ssl(connection))
	 && !(err = make_socket(connection, port, url)))
	return 0;
  return err;
}

int disconnect_apn(uapn_connection *c)
{
  if(!c)
	return 0;
  if(SSL_shutdown(c->ssl.ssl) == -1)
	return error(UAPN_ERROR_SSL_SHUTDOWN);
  if(close(c->ssl.sock) == -1)
	return error(UAPN_ERROR_CLOSING_SOCKET);
  SSL_free(c->ssl.ssl);
  SSL_CTX_free(c->ssl.ctx);
  return 0;
}

int connect_apn(uapn_connection *c)
{
  char *url;

  switch(c->environement)
	{
	case(UAPN_PRODUCTION_ENVIRONMENT):
	  url = UAPN_SEND_PRODUCTION;
	  break;
	case(UAPN_DEVELOPMENT_ENVIRONMENT):
	  url = UAPN_SEND_DEVELOPMENT;
	  break;
	default: abort();
	}
  return connect_to_service(c, url, UAPN_SEND_PORT);
}

int send_notification(uapn_connection *c, uapn_notification *n)
{
  token *token;
  char buff[1024]; // close enought ;)
  unsigned int length;

  for(token = n->first_token; token; token = token->next)
	{
	  switch(n->format)
		{
		case (UAPN_SIMPLE_FORMAT):
		  length = make_simple_msg(buff, token, n);
		  break;
		case (UAPN_ENHANCED_FORMAT):
		  break;
		}
	  write(1, buff, length);
	  
	  if(SSL_write(c->ssl.ssl, buff, length) > 0)
		return error(UAPN_ERROR_SOCKET_WRITE);
	}  
  return 0;
}
