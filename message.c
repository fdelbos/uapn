//
//  message.c
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#include "uapn.h"

static void hex_to_bin(char *hexa_str, char *output)
{
  char *digits = "0123456789abcdef";
  char tmp;

  for(; hexa_str; hexa_str += 2, output++)
	{
	  tmp = (strchr(digits, *hexa_str) - digits) * 16;
	  tmp = strchr(digits, *(hexa_str + 1)) - digits;
	  *output = tmp;
	}
}

int make_simple_msg(char *msg, token *t, uapn_notification *n)
{
  //char binary_token[32];
  char *begin = msg;
  uint8_t command = 0; // command number
  uint16_t token_size = htons(UAPN_TOKEN_SIZE);
  uint16_t json_size = htons(strlen(n->json));
  
  *msg++ = command;
  
  memcpy(msg, &token_size, sizeof(uint16_t));
  msg += sizeof(uint16_t);

  hex_to_bin(t->token, msg);
  msg += UAPN_TOKEN_SIZE;
  
  memcpy(msg, &json_size, sizeof(uint16_t));
  msg += sizeof(uint16_t);
  
  memcpy(msg, n->json, json_size);
  msg += json_size;
  
  return msg - begin;
}
