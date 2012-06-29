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

static int setup_ssl(char *cert, char *key, ssl_connection *c)
{
  SSL_library_init();
  SSL_load_error_strings();
  c->method = SSLv3_method();
  if(!(c->ctx = SSL_CTX_new(c->method)))
	return error(ERROR_SSL_CONTEXT, -1);
  //if(SSL_CTX_load_verify_locations(c->ctx, NULL, ))
  if (SSL_CTX_use_certificate_file(c->ctx, cert, SSL_FILETYPE_PEM) <= 0)
	return error(ERROR_INVALID_CERT, -1);
  if (SSL_CTX_use_PrivateKey_file(c->ctx, key, SSL_FILETYPE_PEM) <= 0)
	return error(ERROR_INVALID_KEY, -1);
  if (!SSL_CTX_check_private_key(c->ctx))
	return error(ERROR_CERT_KEY_MISMATCH, -1);
  if((c->sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	return error(ERROR_SOCKET, -1);

  return 0;
}

static int make_socket(notification *n, ssl_connection *c)
{
  memset (&c->server_addr, 0, sizeof(c->server_addr));
  c->server_addr.sin_family = AF_INET;
  c->server_addr.sin_port = htons(APN_PORT);
  switch(n->version)
	{
	case (PRODUCTION):
	  c->host_info = gethostbyname(APN_PRODUCTION);
	  break;
	case (DEVELOPMENT):
	  c->host_info = gethostbyname(APN_DEVELOPMENT);
	default:
	  abort();
	}
  if(!c->host_info)
	return error(ERROR_HOSTNAME, 0);

  struct in_addr *address = (struct in_addr*)c->host_info->h_addr_list[0];
  c->server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*address));
  if(connect(c->sock, (struct sockaddr*) &c->server_addr, sizeof(c->server_addr)) == -1)
	return error(ERROR_CONNECT, -1);
  if(!(c->ssl = SSL_new(c->ctx)))
	return error(ERROR_SSL_SOCKET, -1);
  SSL_set_fd(c->ssl, c->sock);
  if(SSL_connect(c->ssl) <= -1)
	return error(ERROR_SSL_CONNECT, -1);
  
  return 0;
}

int send_notification(notification *n)
{
  ssl_connection ssl_c;
  
  if(setup_ssl(n->cert_path, n->key_path, &ssl_c)
	 || make_socket(n, &ssl_c))
	return -1;
  printf("send ready\n");
  return 0;
}
