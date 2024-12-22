/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      handlers.c                                                    */
/*                                                                            */
/* Author:      The Programmers' Guild, Inc.                                  */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Apache module to support Rexx/Object Rexx.                    */
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


#include "mod_rexx.h"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_module_init                                              */
/*                                                                            */
/* Description: This handler is invoked when a new the module is created.     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_post_config_handler(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp,
                      server_rec *s)
{

    ap_add_version_component(p, version);
    mod_rexx_global_table = apr_table_make(p, 10);
    return 0;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_handler                                                  */
/*                                                                            */
/* Description: Main content handler. This function calls Rexx to handle the  */
/*              HTTP request.                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_handler(request_rec *r)
{
    int rxrc, rxpgmrc;
    char *rxprocpath;
    rexx_config *c;
    char rxarg[64];

    /* test for our MIME types */
    if(strcmp(r->handler, "application/x-httpd-rexx-script")
        && strcmp(r->handler, "rexx-handler")
        && strcmp(r->handler, "rexx_handler")) {
        return DECLINED;
    }

//  modrexx_debug(r->server, "Entering rexx_handler routine.");

    /* If we're only supposed to send header information (HEAD request), */
    /* then don't bother Rexx with the request.                          */
    if (r->header_only) {
        /* Set the content-type */
        r->content_type = "text/html";
        ap_send_http_header(r);
//      modrexx_debug(r->server, "Exiting rexx_handler routine.");
        return OK;
    }

    /* If the Rexx/RSP file does not exist then return an error. */
    if(r->finfo.filetype == 0) {
        return HTTP_NOT_FOUND;
    }

    /* Are we overridding the proc name through a config directive? */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &rexx_module);
    if (c->content != NULL) {
        rxprocpath = c->content;
    }
    else {
        rxprocpath = r->filename;
    }

    /* call the Rexx interpreter */
    sprintf(rxarg, "%p", r); /* never pass binary data to RexxStart. */
    rxpgmrc = Call_Rexx(r, rxprocpath, rxarg, WWW_IO_EXIT, &rxrc);

    /* test return code and log error if necessary. */
    if (rxrc != 0) {
        rexxstart_error_processor(r, rxprocpath, rxrc);
        rxpgmrc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_handler routine.");

    /* Cleanup */
    return (int)rxpgmrc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_rsphandler                                               */
/*                                                                            */
/* Description: Handler for Rexx RSP files. This fuction calls Rexx to handle */
/*              an RSP request.                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_rsphandler(request_rec *r)
{
    int rxrc, rxpgmrc;
    rexx_config *c;
    char * TempName;
    char *rxarg;

    /* test for our MIME type */
    if(strcmp(r->handler, "application/x-httpd-rexx-rsp")) {
        return DECLINED;
    }

//  modrexx_debug(r->server, "Entering rexx_rsphandler routine.");

    /* If we're only supposed to send header information (HEAD request), */
    /* then don't bother Rexx with the request.                          */
    if (r->header_only) {
        /* Set the content-type */
        r->content_type = "text/html";
        ap_send_http_header(r);
//      modrexx_debug(r->server, "Exiting rexx_rsphandler routine.");
        return OK;
    }

    /* If the rsp file does not exist then return an error. */
    if(r->finfo.filetype == 0) {
        return HTTP_NOT_FOUND;
    }

    /* Get a temporary file to put the Rexx program into. */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                                &rexx_module);
    TempName = CreateTempFile(r, (char *) c->fnametemplate);

    /* call the Rexx interpreter to compile the rsp file. */
    rxarg = (char *)apr_pcalloc(r->pool,
                                 strlen(TempName) + strlen(r->filename) + 6);
    sprintf(rxarg, "\"%s\" \"%s\"", r->filename, TempName);
    rxpgmrc = Call_Rexx(r, c->rspcompiler, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code, log error and return if necessary. */
    if (rxrc != 0) {
        rexxstart_error_processor(r, c->rspcompiler, rxrc);
        rxpgmrc = HTTP_INTERNAL_SERVER_ERROR;
        remove(TempName);
//      modrexx_debug(r->server, "Exiting rexx_rsphandler routine.");
        return (int)rxpgmrc;
    }

    /* call the Rexx interpreter to execute the compiled rsp file. */
    if (strlen(rxarg) < 64)
        {
        rxarg = (char *)apr_pcalloc(r->pool, 64);
        }
    sprintf(rxarg, "%p", r); /* never pass binary data to RexxStart. */
    rxpgmrc = Call_Rexx(r, TempName, rxarg, WWW_IO_EXIT, &rxrc);

    /* test return code and log error if necessary. */
    if (rxrc != 0) {
        rexxstart_error_processor(r, c->rspcompiler, rxrc);
        rxpgmrc = HTTP_INTERNAL_SERVER_ERROR;
    }

    /* Cleanup */
    remove(TempName);

//  modrexx_debug(r->server, "Exiting rexx_rsphandler routine.");

    return (int)rxpgmrc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_translation_handler                                      */
/*                                                                            */
/* Description: This handler is invoked during a request. It can translate    */
/*              the URI in the request.                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_translation_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return rc;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &rexx_module);
    if (c->translate == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_translation_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->translate, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_translation_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_authentication_handler                                   */
/*                                                                            */
/* Description: This handler is invoked during a request. Authentication of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_authentication_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &rexx_module);
    if (c->authenticate == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_authentication_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->authenticate, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"        
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_authentication_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_authorization_handler                                    */
/*                                                                            */
/* Description: This handler is invoked during a request. Authorization  of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_authorization_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &rexx_module);
    if (c->authorize == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_authorization_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->authorize, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_authorization_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_access_handler                                           */
/*                                                                            */
/* Description: This handler is invoked during a request. Access control of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_access_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &rexx_module);
    if (c->access == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_access_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->access, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_access_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_mime_type_handler                                        */
/*                                                                            */
/* Description: This handler is invoked during a request. Mime checking of    */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_mime_type_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &rexx_module);
    if (c->mime_type == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_mime_type_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->mime_type, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_mime_type_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_fixup_handler                                            */
/*                                                                            */
/* Description: This handler is invoked during a request. Fix up of           */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_fixup_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &rexx_module);
    if (c->fixup == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_fixup_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->fixup, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_fixup_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_logging_handler                                          */
/*                                                                            */
/* Description: This handler is invoked during a request. Logging of          */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_logging_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &rexx_module);
    if (c->logging == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_logging_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->logging, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_logging_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_header_parser_handler                                    */
/*                                                                            */
/* Description: This handler is invoked during a request. Header parsing of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_header_parser_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &rexx_module);
    if (c->header_parser == NULL) {
        return rc;
    }

//  modrexx_debug(r->server, "Entering rexx_header_parser_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->header_parser, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_header_parser_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_post_request_handler                                     */
/*                                                                            */
/* Description: This handler is invoked during a request. Post processing of  */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int rexx_post_request_handler(request_rec *r)
{
    int rxrc;
    rexx_config *c;
    int rc = DECLINED;
    char rxarg[64];

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &rexx_module);
    if (c->post_read == NULL) {
        return DECLINED;
    }

//  modrexx_debug(r->server, "Entering rexx_post_request_handler routine.");

    /* call Rexx interpreter */
    sprintf(rxarg, "%p", r);
    rc = (int)Call_Rexx(r, c->post_read, rxarg, WWW_IO_EXIT_2, &rxrc);

    /* test return code and log error if necessary */
    if (rxrc != 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        modrexx_error(r->server, get_rexx_err_str(r, rxrc));
#pragma GCC diagnostic pop
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

//  modrexx_debug(r->server, "Exiting rexx_post_request_handler routine.");

    return rc;
}

