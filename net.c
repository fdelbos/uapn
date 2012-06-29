//
//  net.c
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#include <openssl/ssl.h>

#include "uapn.h"


static SSL_Connection *make_ssl_connection(char *cert, char *key)
{
  SSL_Connection *c = xmalloc(sizeof(SSL_Connection));

  SSL_library_init();
  SSL_load_error_strings();
  c->meth = SSLv3_method();
  if(!(c->ctx = SSL_CTX_new(c->meth)))
	return error(ERROR_SSL_CONTEXT, 0);
  //if(SSL_CTX_load_verify_locations(c->ctx, NULL, ))
  if (SSL_CTX_use_certificate_file(c->ctx, n->cert_path, SSL_FILETYPE_PEM) <= 0)
	return error(ERROR_INVALID_CERT, 0);
  if (SSL_CTX_use_PrivateKey_file(c->ctx, n->key_path, SSL_FILETYPE_PEM) <= 0)
	return error(ERROR_INVALID_KEY, 0);
  if (!SSL_CTX_check_private_key(c->ctx))
	return error(ERROR_CERT_KEY_MISMATCH, 0);
  if((c->sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	return error(ERROR_SOCKET, 0);
  
  return c;
}

static int connect(notification *n);
{
  
  memset (&c->server_addr, 0, sizeof(c->server_addr));
  c->server_addr.sin_family = AF_INET;
  c->server_addr.sin_port = htons(APN_PORT);
  switch(c->version)
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
	return error(ERROR_HOSTNAME, -1);

  struct in_addr *address = (struct in_addr*)c->host_info->h_addr_list[0];
  c->server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*address));
}
