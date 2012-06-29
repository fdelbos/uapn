//
//  message.c
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#include "uapn.h"


void token_to_binary(char *token, char *bin)
{
  char tmp[2];
  char res;
  int i;

  for(i = 0; i < TOKEN_SIZE; i++)
	{
	  tmp[0] = token[i * 2];
	  tmp[1] = token[i * 2 + 1];
	  sscanf(tmp, "%x", res);
	  bin[i] = res;
	}
}
