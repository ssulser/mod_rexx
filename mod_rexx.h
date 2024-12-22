/*-- H -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      mod_rexx.h                                                    */
/*                                                                            */
/* Author:      Terry Fuller                                                  */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Include file for Apache module to support Rexx/Object Rexx.   */
/*                                                                            */
/* Copyright (C) The Programmers' Guild, Inc. 2021. All Rights Reserved.      */
/* Copyright (C) W. David Ashley 2004-2007. All Rights Reserved.              */
/*                                                                            */
/* This software is subject to the terms of the Commom Public License. You    */
/* must accept the terms of this license to use this software. Refer to       */
/* the file CPLv1.0.htm included in this package for more information.        */
/*                                                                            */
/* The program is provided "as is" without any warranty express or implied,   */
/* including the warranty of non-infringement and the implied warranties of   */
/* merchantibility and fitness for a particular purpose.                      */
/*                                                                            */
/* Note: The code in this file has been formatted in strict conformance with  */
/* the Apache Developer's C Language Style Guide.                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#ifndef MOD_REXX_H
#define MOD_REXX_H

/* define this to get NT support */
#if defined(WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0400
    #endif
#endif

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif
#if defined(WIN32)
#include <ws2tcpip.h>
#endif

#define INCL_RXFUNC
#define INCL_RXSHV
#define INCL_RXSYSEXIT
#ifdef USE_REGINA
    #include <rexxsaa.h>
#elif defined(USE_REXXTRANS)
    #include <rexxtrans.h>
#else /* must be ooRexx  or IBM Object Rexx */
    #include <oorexxapi.h>  /* was <rexx.h> */
#endif
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(WIN32)
    #include <io.h>
#endif

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_main.h"
#include "http_request.h"
#include "http_protocol.h"
#include "util_script.h"
#include "ap_compat.h"
#include "apr_strings.h"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Global Definitions                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Mod_Rexx version string */
#define MOD_REXX_VERSION "Mod_Rexx/2.4.0"

/* Debug and error log macros */
#define modrexx_debug(srvr, msg) ap_log_error(APLOG_MARK, \
    APLOG_NOERRNO | APLOG_DEBUG, 0, srvr, msg)
#define modrexx_error(srvr, msg) ap_log_error(APLOG_MARK, \
    APLOG_NOERRNO | APLOG_ERR, 0, srvr, msg)

/* Rexx Address and Sysexit names */
#define WWW_INI_EXIT  "WWW_INI_EXIT"
#define WWW_IO_EXIT   "WWW_IO_EXIT"
#define WWW_IO_EXIT_2 "WWW_IO_EXIT_2"
#define RX_ADDR_ENV   "WWW"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Data declarations.                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

extern const command_rec rexx_cmds[];
extern const char *version;
extern const char *compDate;
extern const char *compTime;
extern apr_table_t *mod_rexx_global_table;
extern module MODULE_VAR_EXPORT rexx_module;

/* Linked list struct for the RexxSetVar directive */
typedef struct _rexx_var_rec {
    struct _rexx_var_rec *next;
    char *rexx_var_name;
    char *rexx_var_value;
} rexx_var_rec;

/* Configuration directive structure */
typedef struct _rexx_config {
    struct _rexx_var_rec *rexxvars;
    char *translate;
    char *authenticate;
    char *authorize;
    char *access;
    char *mime_type;
    char *fixup;
    char *logging;
    char *header_parser;
    char *post_read;
    char *content;
    char *rspcompiler;                 /* path to the RSP compiler            */
    char *fnametemplate;               /* RSP temp file template              */
} rexx_config;

/* default RSP compiler and file name template */
#define RSP_COMPILER "rspcomp.rex"
#define RSP_FNAMETEMPLATE "./execrsp?????.rex"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Utility function declarations.                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int SetRexxVar(const char * name, char * value);
void *create_rexx_dir_config(apr_pool_t *p, char *path);
char *get_rexx_err_str(request_rec *r, int rxrc);
void rexxstart_error_processor(request_rec * r, char *rxprocpath, int rxrc);
int Call_Rexx(request_rec *r, char *rxprocpath, char *rstrptr, char *io_exit,
               int *rxrc);
const char * CreateTempFile(request_rec *r, char *fntemplate);
request_rec *GetReqRecPtr(void);


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Handler and module init function declarations.                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int APIENTRY rexx_post_config_handler(apr_pool_t *p, apr_pool_t *plog,
                             apr_pool_t *ptemp, server_rec *s);
int APIENTRY rexx_handler(request_rec *r);
int APIENTRY rexx_rsphandler(request_rec *r);
int APIENTRY rexx_translation_handler(request_rec *r);
int APIENTRY rexx_authentication_handler(request_rec *r);
int APIENTRY rexx_authorization_handler(request_rec *r);
int APIENTRY rexx_access_handler(request_rec *r);
int APIENTRY rexx_mime_type_handler(request_rec *r);
int APIENTRY rexx_fixup_handler(request_rec *r);
int APIENTRY rexx_logging_handler(request_rec *r);
int APIENTRY rexx_header_parser_handler(request_rec *r);
int APIENTRY rexx_post_request_handler(request_rec *r);


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx exit function declarations                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

RexxExitHandler Rexx_INI_Exit;
RexxExitHandler Rexx_IO_Exit;
RexxExitHandler Rexx_IO_Exit_2;


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Misc Rexx function declarations                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* The following registers all of the Rexx external functions */
void reg_rexx_ext_funcs(void);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef MOD_REXX_H */


