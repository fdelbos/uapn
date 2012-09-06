//
//  utils.c
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#include "uapn.h"

static error_msg errors[20] =
  {
	{UAPN_ERROR_JSON_SIZE,				"JSON payload size must be at most 256 bytes long."},
	{UAPN_ERROR_TOKEN,					"Token string size must be exaclty 64 bytes long."},
	{UAPN_ERROR_CERT_MANDATORY,			"RSA certificate is mandatory."},
	{UAPN_ERROR_KEY_MANDATORY,			"RSA key is mandatory."},
	{UAPN_ERROR_JSON_MANDATORY,			"JSON payload is mandatory."},
	{UAPN_ERROR_NO_TOKEN,				"No token provided."},
	{UAPN_ERROR_SSL_CONTEXT,			"Could not get SSL context."},
	{UAPN_ERROR_INVALID_CERT,			"Invalid RSA certificate."},
	{UAPN_ERROR_INVALID_KEY,			"Invalid RSA key."},
	{UAPN_ERROR_CERT_KEY_MISMATCH,		"Certificate and key don't match."},
	{UAPN_ERROR_SOCKET,					"Can't create socket."},
	{UAPN_ERROR_HOSTNAME,				"Can't resolve hostname."},
	{UAPN_ERROR_SSL_SOCKET,				"Can't create SSL socket."},
	{UAPN_ERROR_SSL_CONNECT,			"Could not connect to SSL server."},
	{UAPN_ERROR_TOKEN_NOT_HEXA,			"Provided token is not a valid hexadecimal number."},
	{UAPN_ERROR_MEMORY,					"Can't alloc memory."},
	{UAPN_ERROR_SOCKET_WRITE,			"SSL can't write notification on socket."},
	{UAPN_ERROR_SSL_SHUTDOWN,			"SSL shutdown was not successful."},
	{UAPN_ERROR_CLOSING_SOCKET,			"Network socket close error."},
	{0, 0}
  };


int error(char id)
{
  int i;
  
  for(i = 0; errors[i].message != 0; i++)
	if(errors[i].id == id)
	  {
		fprintf(stderr, "error: %s\n", errors[i].message);
		break;
	  }
  return id;
}

void *xmalloc(size_t size)
{
  void *ptr = malloc(size);
  
  if(ptr == NULL)
	exit(error(UAPN_ERROR_MEMORY));
  return ptr;
}

char *to_lower(char *str)
{
  int i = 0;
  
  for(;str[i]; i++)
	if(str[i] >= 'A' && str[i] <= 'Z')
	  str[i] -= 32;
  return str;
}
