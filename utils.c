//
//  utils.c
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#include "uapn.h"

static error_msg errors[14] =
  {
	{ERROR_MEMORY,				"Can't alloc memory."},
	{ERROR_JSON_SIZE,			"JSON payload size must be at most 256 bytes long."},
	{ERROR_TOKEN,				"Token string size must be exaclty 64 bytes long."},
	{ERROR_CERT_MANDATORY,		"RSA certificate is mandatory."},
	{ERROR_KEY_MANDATORY,		"RSA key is mandatory."},
	{ERROR_JSON_MANDATORY,		"JSON payload is mandatory."},
	{ERROR_NO_TOKEN,			"No token provided."},
	{ERROR_SSL_CONTEXT,			"Could not get SSL context."},
	{ERROR_INVALID_CERT,		"Invalid RSA certificate."},
	{ERROR_INVALID_KEY,			"Invalid RSA key."},
	{ERROR_CERT_KEY_MISMATCH,	"Certificate and key don't match."},
	{ERROR_SOCKET,				"Can't create socket."},
	{ERROR_HOSTNAME,			"Can't resolve hostname."},
	{0, 0}
  };


int error(char id, int return_value)
{
  int i;
  
  for(i = 0; errors[i].message != 0; i++)
	if(errors[i].id == id)
	  {
		fprintf(stderr, "error: %s\n", errors[i].message);
		break;
	  }
  return return_value;
}

int display_usage()
{
  const char *usage = 
	"usage: ./uapn -c rsa_client_cert -k rsa_clien_key "
	"-j json_payload device_token ...\n";
  fprintf(stderr, "%s", usage);
  return -1;
}

void *xmalloc(size_t size)
{
  void *ptr = malloc(size);
  
  if(ptr == NULL)
	exit(error(ERROR_MEMORY, -1));
  return ptr;
}