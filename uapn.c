//
//  uapn.c
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "uapn.h"

static int is_hexa_str(char *str)
{
  int i;
  
  for(i = 0; str[i]; i++)
	if(!(str[i] >= '0' && str[i] <= '9') && !(str[i] >= 'a' && str[i] <= 'f'))
	  return 0;
  return 1;
}

static token *make_token()
{
  token *t = xmalloc(sizeof(token));
  t->token = 0;
  t->next = 0;
  return t;
}

static void init_notification(uapn_notification *n)
{
  n->expiry_date = 0;
  n->json = 0;
  n->format = UAPN_SIMPLE_FORMAT;
  n->first_token = 0;
}

static void init_connection(uapn_connection *c)
{
  c->cert_path = c->key_path = c->password = 0;
  c->environement = UAPN_PRODUCTION_ENVIRONMENT;
}

static int parse_cmd(uapn_notification *n, uapn_connection *c, int ac, char **av)
{
  int arg;
  token *current = 0;
  
  while ((arg = getopt(ac, av, "dpset:a:")) != -1)
	{
	  switch (arg)
		{
		case 'd': c->environement = UAPN_DEVELOPMENT_ENVIRONMENT;
		  break;
		case 'p': c->environement = UAPN_PRODUCTION_ENVIRONMENT;
		  break;
		case 's': n->format = UAPN_SIMPLE_FORMAT;
		  break;
		case 'e': n->format = UAPN_ENHANCED_FORMAT;
		  break;
		case 't': n->expiry_date = atoi(optarg);
		  break;
		case 'a': c->password = optarg;
		  break;
		case '?': return display_usage();
		default:
		  abort();
		}
	}
  if(optind < ac)
	c->cert_path = av[optind++];
  if(optind < ac)
	c->key_path = av[optind++];
  if(optind < ac)
	{
	  n->json = av[optind++];
	  if(strlen(n->json) > UAPN_MAX_MESSAGE_SIZE)
		return error(UAPN_ERROR_JSON_SIZE);
	}	
  while(optind < ac)
	{
	  if(!current)
		n->first_token = current = make_token();
	  else
		current = current->next = xmalloc(sizeof(token));
	  if(strlen(current->token = to_lower(av[optind++])) != 64)
		return error(UAPN_ERROR_TOKEN);
	  if(!is_hexa_str(current->token))
		return error(UAPN_ERROR_TOKEN_NOT_HEXA);
	}
  if(!c->cert_path)
	return error(UAPN_ERROR_CERT_MANDATORY);
  if(!c->key_path)
	return error(UAPN_ERROR_KEY_MANDATORY);
  if(!n->json)
	return error(UAPN_ERROR_JSON_MANDATORY);
  if(!n->first_token)
	return error(UAPN_ERROR_NO_TOKEN);
  
  return 0;
}

int display_usage()
{
  const char *usage = 
	"usage: ./uapn [-dpseta] rsa_client_cert rsa_client_key "
	"json_payload device_token [...]\n";
  fprintf(stderr, "%s", usage);
  return -1;
}

int main(int ac, char **av)
{
  uapn_notification n;
  uapn_connection c;
  
  init_notification(&n);
  init_connection(&c);
  if(parse_cmd(&n, &c, ac, av))
	  return -1;
  connect_apn(&c);
  send_notification(&c, &n);
  disconnect_apn(&c);
  return 0;
}
