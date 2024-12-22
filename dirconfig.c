/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      dirconfig.c                                                   */
/*                                                                            */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Apache module to support Rexx/Object Rexx.                    */
/*                                                                            */
/* Copyright (C) The Programmers' Guild, Inc. 2021 All Rights Reserved.       */
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


#include "mod_rexx.h"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    create_rexx_dir_config                                        */
/*                                                                            */
/* Description: Creates the pool used by the directive processor functions.   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

void *create_rexx_dir_config(apr_pool_t *p, char *path)
{
    rexx_config *c;

    /* Allocate memory for the config record */
    c = (rexx_config *)apr_pcalloc(p, sizeof(rexx_config));

    /* Initialize the data fields */
    c->rexxvars = NULL;
    c->translate = NULL;
    c->authenticate = NULL;
    c->authorize = NULL;
    c->access = NULL;
    c->mime_type = NULL;
    c->fixup = NULL;
    c->logging = NULL;
    c->header_parser = NULL;
    c->post_read = NULL;
    c->content = NULL;
    c->rspcompiler = RSP_COMPILER;
    c->fnametemplate = RSP_FNAMETEMPLATE;

    /* Successful processing */
    return (void *)c;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_set_var                                                  */
/*                                                                            */
/* Description: Process a directive which sets the value of a Rexx variable   */
/*              that can be used in any handler.                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_set_var(cmd_parms *parms, void *mconfig,
                                const char *varname, const char *value)
{
    rexx_config *c;
    rexx_var_rec *er;
    rexx_var_rec *nr;

    /* Convert pointers */
    c = (rexx_config *) mconfig;

    /* Find the place in the linked list to put our RexxSetVar information    */
    /* and allocate a new struct to hold the directive                        */
    er = c->rexxvars;
    if (er == NULL) {
        nr = (rexx_var_rec *)apr_pcalloc(parms->pool, sizeof(rexx_var_rec));
        c->rexxvars = nr;
        }
    else {
        while (er->next != NULL)
           er = er->next;
        nr = (rexx_var_rec *)apr_pcalloc(parms->pool, sizeof(rexx_var_rec));
        er->next = nr;
        }

    /* Now that we have a new record we can initialize it */
    nr->next = NULL;
    nr->rexx_var_name = apr_pstrdup(parms->pool, varname);
    nr->rexx_var_value = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_post_read                                                */
/*                                                                            */
/* Description: Process a directive which sets the Post Read Request Handler. */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_post_read(cmd_parms *parms, void *mconfig,
                                  const char *value)
{
    rexx_config  *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->post_read = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_trans                                                    */
/*                                                                            */
/* Description: Process a directive which sets the Translation Handler.       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_trans(cmd_parms *parms, void  *mconfig,
                              const char *value)
{
    rexx_config  *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->translate = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_header_parser                                            */
/*                                                                            */
/* Description: Process a directive which sets the Header Parser Handler.     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_header_parser(cmd_parms *parms, void *mconfig,
                                      const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->header_parser = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_access                                                   */
/*                                                                            */
/* Description: Process a directive which sets the Access Handler.            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_access(cmd_parms *parms, void *mconfig,
                               const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->access = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_authen                                                   */
/*                                                                            */
/* Description: Process a directive which sets the Authentication Handler.    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_authen(cmd_parms *parms, void *mconfig,
                               const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->authenticate = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_authz                                                    */
/*                                                                            */
/* Description: Process a directive which sets the Authorization Handler.     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_authz(cmd_parms *parms, void *mconfig,
                              const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->authorize = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_type                                                     */
/*                                                                            */
/* Description: Process a directive which sets the Type Checking Handler.     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_type(cmd_parms *parms, void *mconfig,
                             const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->mime_type = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_fixup                                                    */
/*                                                                            */
/* Description: Process a directive which sets the Fixup Handler.             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_fixup(cmd_parms *parms, void *mconfig,
                              const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->fixup = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_log                                                      */
/*                                                                            */
/* Description: Process a directive which sets the Logging Handler.           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_log(cmd_parms *parms, void *mconfig,
                            const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *) mconfig;

    /* allocate a new struct to hold the directive */
    c->logging = apr_pstrdup (parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_content                                                  */
/*                                                                            */
/* Description: Process a directive which sets the Content Handler.           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_content(cmd_parms *parms, void *mconfig,
                                const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->content = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_rsp_compiler                                             */
/*                                                                            */
/* Description: Process a directive which sets the RSP compier.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_rsp_compiler(cmd_parms *parms, void *mconfig,
                                const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->rspcompiler = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_fnametemplate                                            */
/*                                                                            */
/* Description: Process a directive which sets the SysTemFileName template.   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static const char *rexx_fnametemplate(cmd_parms *parms, void *mconfig,
                                const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->fnametemplate = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The list of Apache configuration directives available for Mod_Rexx.        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

const command_rec rexx_cmds[] =
{
    {"RexxSetVar", (cmd_func) rexx_set_var, NULL, OR_ALL, TAKE2,
     "A Rexx variable name followed by the value."},
    {"RexxPostReadRequestHandler", (cmd_func) rexx_post_read, NULL, OR_ALL, TAKE1,
     "The name of the Post Read Request Handler."},
    {"RexxTransHandler", (cmd_func) rexx_trans, NULL, OR_ALL, TAKE1,
     "The name of the Translation Handler."},
    {"RexxHeaderParserHandler", (cmd_func) rexx_header_parser, NULL, OR_ALL, TAKE1,
     "The name of the Header Parser Handler."},
    {"RexxAccessHandler", (cmd_func) rexx_access, NULL, OR_ALL, TAKE1,
     "The name of the Access Handler."},
    {"RexxAuthenHandler", (cmd_func) rexx_authen, NULL, OR_ALL, TAKE1,
     "The name of the Authentication Handler."},
    {"RexxAuthzHandler", (cmd_func) rexx_authz, NULL, OR_ALL, TAKE1,
     "The name of the Authorization Handler."},
    {"RexxTypeHandler", (cmd_func) rexx_type, NULL, OR_ALL, TAKE1,
     "The name of the Type Checking Handler."},
    {"RexxFixupHandler", (cmd_func) rexx_fixup, NULL, OR_ALL, TAKE1,
     "The name of the Fixup Handler."},
    {"RexxLogHandler", (cmd_func) rexx_log, NULL, OR_ALL, TAKE1,
     "The name of the Logging Handler."},
    {"RexxHandler", (cmd_func) rexx_content, NULL, OR_ALL, TAKE1,
     "The name of the Content Handler."},
    {"RexxRspCompiler", (cmd_func) rexx_rsp_compiler, NULL, OR_ALL, TAKE1,
     "The name of the Rexx script which compiles RSP files."},
    {"RexxTempFileNameTemplate", (cmd_func) rexx_fnametemplate, NULL, OR_ALL, TAKE1,
     "The template to be passed to the SysTempFileName external function."},
    {NULL}
};

