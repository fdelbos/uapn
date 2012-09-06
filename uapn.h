//
//  uapn.h
//  uapn
//
//  Created by Frederic Delbos on 6/28/12.
//  Copyright (c) 2012 Frederic Delbos Conseil Informatique. All rights reserved.
//

#ifndef __UAPN_H__
#define __UAPN_H__

#define UAPN_SEND_PRODUCTION					"gateway.push.apple.com"
#define UAPN_SEND_DEVELOPMENT					"gateway.sandbox.push.apple.com"
#define UAPN_SEND_PORT							2195

#define UAPN_FEEDBACK_PRODUCTION				"feedback.push.apple.com"
#define UAPN_FEEDBACK_DEVELOPMENT				"feedback.sandbox.push.apple.com"
#define UAPN_FEEDBACK_PORT						2196

#define UAPN_PRODUCTION_ENVIRONMENT				0
#define	UAPN_DEVELOPMENT_ENVIRONMENT			1

#define UAPN_SIMPLE_FORMAT						0
#define UAPN_ENHANCED_FORMAT					1

#define UAPN_TOKEN_SIZE							32
#define UAPN_MAX_MESSAGE_SIZE					255

#define UAPN_ERROR_JSON_SIZE					1
#define UAPN_ERROR_TOKEN						2
#define UAPN_ERROR_CERT_MANDATORY				3
#define UAPN_ERROR_KEY_MANDATORY				4
#define UAPN_ERROR_JSON_MANDATORY				5
#define UAPN_ERROR_NO_TOKEN						6
#define UAPN_ERROR_SSL_CONTEXT					7
#define UAPN_ERROR_INVALID_CERT					8
#define UAPN_ERROR_INVALID_KEY					9
#define UAPN_ERROR_CERT_KEY_MISMATCH			10
#define UAPN_ERROR_SOCKET						11
#define UAPN_ERROR_HOSTNAME						12
#define UAPN_ERROR_CONNECT						13
#define UAPN_ERROR_SSL_SOCKET					14
#define UAPN_ERROR_SSL_CONNECT					15
#define UAPN_ERROR_TOKEN_NOT_HEXA				16
#define UAPN_ERROR_MEMORY						17
#define UAPN_ERROR_SOCKET_WRITE					18
#define UAPN_ERROR_SSL_SHUTDOWN					19
#define UAPN_ERROR_CLOSING_SOCKET				20

#include <sys/socket.h>
#include <sys/types.h>
#include <openssl/ssl.h>
#include <netinet/in.h>


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

typedef struct t_uapn_notification
{
  char	*json;
  char	format;
  int	expiry_date;
  token *first_token;  
} uapn_notification;

typedef struct s_uapn_ssl
{
  SSL_CTX				*ctx;
  SSL					*ssl;
  SSL_METHOD			*method;
  X509					*server_cert;
  EVP_PKEY				*pkey;  
  struct sockaddr_in	server_addr;
  struct hostent		*host_info;
  int					sock;
} uapn_ssl;

typedef struct t_uapn_connection
{
  uapn_ssl ssl;
  char	environement;
  char	*cert_path;
  char	*key_path;
  char	*password;
} uapn_connection;


//
//		MESSAGE.C
//
int make_simple_msg(char *msg, token *t, uapn_notification *n);

//
//		NET.C
//
int connect_apn(uapn_connection *c);
int send_notification(uapn_connection *c, uapn_notification *n);
int disconnect_apn(uapn_connection *c);

//
//		UTILS.C
//
int error(char id);
int display_usage();
void *xmalloc(size_t size);
char *to_lower(char *str);


#endif
