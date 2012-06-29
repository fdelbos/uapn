//
//  uapn.h
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#ifndef __UAPN_H__
#define __UAPN_H__

#define APN_PRODUCTION			"gateway.push.apple.com"
#define APN_DEVELOPMENT			"gateway.sandbox.push.apple.com"
#define APN_PORT				2195

#define PRODUCTION				0
#define	DEVELOPMENT				1

#define ERROR_MEMORY			0
#define ERROR_JSON_SIZE			1
#define ERROR_TOKEN				2
#define ERROR_CERT_MANDATORY	3
#define ERROR_KEY_MANDATORY		4
#define ERROR_JSON_MANDATORY	5
#define ERROR_NO_TOKEN			6
#define ERROR_SSL_CONTEXT		7
#define ERROR_INVALID_CERT		8
#define ERROR_INVALID_KEY		9
#define ERROR_CERT_KEY_MISMATCH	10
#define ERROR_SOCKET			11
#define ERROR_HOSTNAME			12


typedef struct t_error_msg
{
  int	id;
  char	*message;
} error_msg;

typedef struct t_token
{
  char	*token;
  struct t_token *next;
} token;

typedef struct t_notification
{
  char	*cert_path;
  char	*key_path;
  char	*json;
  int	version;
  token *first_token;
} notification;

#endif
