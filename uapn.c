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

static int parse_cmd(notification *n, int ac, char **av)
{
  int c;
  token *current = 0;
  
  n->cert_path = n->key_path = n->json = 0;
  n->version = PRODUCTION;
  n->first_token = 0;

  while ((c = getopt(ac, av, "d")) != -1)
	{
	  switch (c)
		{
		case 'd': n->version = DEVELOPMENT;
		  break;
		case '?': return display_usage();
		default:
		  abort();
		}
	}
  if(optind < ac)
	n->cert_path = av[optind++];
  if(optind < ac)
	n->key_path = av[optind++];
  if(optind < ac)
	{
	  n->json = av[optind++];
	  if(strlen(n->json) > 256)
		return error(ERROR_JSON_SIZE, -1);
	}	
  while(optind < ac)
	{
	  if(!current)
		n->first_token = current = make_token();
	  else
		current = current->next = xmalloc(sizeof(token));
	  if(strlen(current->token = to_lower(av[optind++])) != 64)
		return error(ERROR_TOKEN, -1);
	  if(!is_hexa_str(current->token))
		return error(ERROR_TOKEN_NOT_HEXA, -1);
	}
  if(!n->cert_path)
	return error(ERROR_CERT_MANDATORY, -1);
  if(!n->key_path)
	return error(ERROR_KEY_MANDATORY, -1);
  if(!n->json)
	return error(ERROR_JSON_MANDATORY, -1);
  if(!n->first_token)
	return error(ERROR_NO_TOKEN, -1);
  
  return 0;
}

int main(int ac, char **av)
{
  notification n;

  if(parse_cmd(&n, ac, av))
	  return -1;
  send_notification(&n);
  return 0;
}
