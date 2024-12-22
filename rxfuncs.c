/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      rxfuncs.c                                                     */
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
/* Local definitions                                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define RXFUNC_BADCALL 40
#define PRXFUNC RexxFunctionHandler *


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions are general and implement high-level Apache API    */
/* functionality or specific mod_rexx functionality.                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWGetVersion                                      */
/*                                                                            */
/* Description: Rexx callable external function. Gets the Mod_Rexx version    */
/*              string.                                                       */
/*                                                                            */
/* Rexx Args:   None                                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4 
ULONG APIENTRY WWWGetVersion(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
 */
APIRET APIENTRY WWWGetVersion(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{

    /* Check for valid arguments */
    if (Argc != 0) {
       return RXFUNC_BADCALL;
    }

    /* Set the return code */
    strcpy(Retstr->strptr, version);
    strcat(Retstr->strptr, " ");
    strcat(Retstr->strptr, compDate);
    strcat(Retstr->strptr, " ");
    strcat(Retstr->strptr, compTime);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWLogError                                        */
/*                                                                            */
/* Description: Rexx callable external function. Logs an error level          */
/*              message.                                                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWLogError(PSZ Name, LONG Argc, RXSTRING Argv[],
                           PSZ Queuename, PRXSTRING Retstr)
 */
APIRET APIENTRY WWWLogError(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
       return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

    /* Log the error message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0, r, "%s",
                  (char *) Argv[1].strptr);

    /* Set the return code */
    strcpy(Retstr->strptr, "\0");
    Retstr->strlength = 0;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWLogWarning                                      */
/*                                                                            */
/* Description: Rexx callable external function. Logs a warning level         */
/*              message.                                                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWLogWarning(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
 */
APIRET APIENTRY WWWLogWarning(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
       return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

    /* Log the error message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_WARNING, 0, r, "%s",
                  (char *) Argv[1].strptr);

    /* Set the return code */
    strcpy(Retstr->strptr, "\0");
    Retstr->strlength = 0;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWLogInfo                                         */
/*                                                                            */
/* Description: Rexx callable external function. Logs a informational level   */
/*              message.                                                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWLogInfo(PSZ Name, LONG Argc, RXSTRING Argv[],
                          PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWLogInfo(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
       return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

    /* Log the error message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_INFO, 0, r, "%s",
                  (char *) Argv[1].strptr);

    /* Set the return code */
    strcpy(Retstr->strptr, "\0");
    Retstr->strlength = 0;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWLogReason                                       */
/*                                                                            */
/* Description: Rexx callable external function. Logs a reason level          */
/*              message.                                                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWLogReason(PSZ Name, LONG Argc, RXSTRING Argv[],
                            PSZ Queuename, PRXSTRING Retstr)
 */
APIRET APIENTRY WWWLogReason(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1]) ||
        !RXVALIDSTRING (Argv[2])) {
       return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

    /* Log the reason message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO, 0, r,
                  "Access to %s failed for %s. reason: %s",
                  Argv[2].strptr,
                  ap_get_remote_host(r->connection, r->per_dir_config,
                                      REMOTE_NAME, NULL),
                  Argv[1].strptr);

    /* Set the return code */
    strcpy(Retstr->strptr, "\0");
    Retstr->strlength = 0;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSendHTTPHeader                                  */
/*                                                                            */
/* Description: Rexx callable external function. Sets the content type for    */
/*              returned HTTP page and send the HTTP header.                  */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Content type                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSendHTTPHeader(PSZ Name, LONG Argc, RXSTRING Argv[],
                                 PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSendHTTPHeader(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWSendHTTPHeader routine.");

    /* Set the content-type and send the header */
    r->content_type = apr_pstrdup(r->pool, Argv[1].strptr);
    ap_send_http_header(r);

    /* Set the return code */
    *(Retstr->strptr) = '\0';
    Retstr->strlength = 0;

//  modrexx_debug(r->server, "Exiting WWWSendHTTPHeader routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWAddCookie                                       */
/*                                                                            */
/* Description: Rexx callable external function. Sets a cookie in the header  */
/*              of the page in the returned HTTP request.                     */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Cookie value                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWAddCookie(PSZ Name, LONG Argc, RXSTRING Argv[],
                            PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWAddCookie(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWAddCookie routine.");

    /* Set the cookie */
    apr_table_add(r->headers_out, "Set-Cookie", RXSTRPTR(Argv[1]));

    /* Set the return code */
    *(Retstr->strptr) = '\0';
    Retstr->strlength = 0;

//  modrexx_debug(r->server, "Exiting WWWAddCookie routine.");

    return RXFUNC_OK;
   }


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSetHeaderValue                                  */
/*                                                                            */
/* Description: Rexx callable external function. Replaces a value in the      */
/*              header of the page in the returned HTTP request.              */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Key value                                                     */
/*              Cookie value                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSetHeaderValue(PSZ Name, LONG Argc, RXSTRING Argv[],
                                 PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSetHeaderValue(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1]) ||
        !RXVALIDSTRING(Argv[2])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWSetHeaderValue routine.");

    /* Set the cookie */
    apr_table_set(r->headers_out, RXSTRPTR(Argv[1]), RXSTRPTR(Argv[2]));

    /* Set the return code */
    *(Retstr->strptr) = '\0';
    Retstr->strlength = 0;

//  modrexx_debug(r->server, "Exiting WWWSetHeaderValue routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    translate                                                     */
/*                                                                            */
/* Description: translate characters in a string.                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static char *translate(char *source, char *intable, char *outtable)
{
    int i = 0, j;

    /* Look for bad input argument data */
    if (source == NULL || intable == NULL || outtable == NULL) {
        return (source);
    }
    if (strlen(source) == 0 || strlen(intable) == 0 ||
        strlen(outtable) == 0) {
        return source;
    }

    /* Perform character translation(s) */
    for (i = 0; i < strlen(source); i++) {
        for (j = 0; j < strlen(intable); j++) {
            if (*(intable + j) == *(source + i)) {
                *(source + i) = *(outtable + j);
                break;
            }
        }
    }

    return source;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    read_post_vars                                                */
/*                                                                            */
/* Description: Read the POST request variables from the client.              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static int read_post_vars(request_rec *r, const char **rbuf)
{
    int rc;

    /* See if we need to chunk the request */
    if ((rc = ap_setup_client_block(r, REQUEST_CHUNKED_ERROR)) != OK) {
        return rc;
    }

    /* Now do the body of the request */
    if (ap_should_client_block (r)) {
        char argsbuffer[HUGE_STRING_LEN];
        int rsize, len_read, rpos = 0;
        long length = r->remaining;

        *rbuf = apr_pcalloc(r->pool, length + 1);

        while ((len_read =
               ap_get_client_block(r, argsbuffer, sizeof(argsbuffer))) > 0) {
            if ((rpos + len_read) > length) {
                rsize = length - rpos;
            }
            else {
                rsize = len_read;
            }
            memcpy((char *)*rbuf + rpos, argsbuffer, rsize);
            rpos += rsize;
        }
    }

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    parse_vars                                                    */
/*                                                                            */
/* Description: Parses the buffer for name=value pairs and then assigns them  */
/*              to Rexx stem variables.                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static void parse_vars(request_rec *r, char *data)
{
    int num = 0;
    char *val;
    char *name;
    char varname[512];

    if (data != NULL) {
        while (*data &&
               (val = ap_getword (r->pool, (const char **) &data, '&'))) {
            name = ap_getword(r->pool, (const char **)&val, '=');
            ap_unescape_url(translate(name, "+", " "));
            ap_unescape_url(translate(val, "+", " "));

            /* We now have a name=value pair so create a Rexx variable */
            num++;
            sprintf(varname, "WWWARGS.%d.!NAME", num);
            SetRexxVar(varname, name);
            sprintf(varname, "WWWARGS.%d.!VALUE", num);
            SetRexxVar(varname, val);
            sprintf(varname, "WWWARGS.%s", name);
            SetRexxVar(varname, val);
        }
    }

    /* Now set the correct number of arguments */
    sprintf(varname, "%d", num);
    SetRexxVar("WWWARGS.0", varname);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWGetArgs                                         */
/*                                                                            */
/* Description: Rexx callable external function. Returns either the GET or    */
/*              POST name/value pairs.                         .              */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWGetArgs(PSZ Name, LONG Argc, RXSTRING Argv[],
                          PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWGetArgs(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    const char *type;
    char *data = NULL;
    int rc;

    /* Check for valid arguments */
    if (Argc == 0) {
        return RXFUNC_BADCALL;
    }
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWGetArgs routine.");

    /* This routine has been prone to errors so flush the buffer to the */
    /* client so we can see something at the client browser.            */
//  ap_rflush(r);
    /* I commented this out because it causes a problem with WWWSendHTTPHeader
     * function sending a default (wrong) header Mime type.
     */

    /* What kind of request is this? */
    switch (r->method_number) {
    case (M_POST):
        /* Test content type */
        type = apr_table_get(r->headers_in, "Content-Type");
        /* There are tools which can send a POST request without a content-type */
        if (type != NULL) {
            if (strcasecmp(type, "application/x-www-form-urlencoded") != 0) {
                break;
            }
        }

        /* Read the request body and point to POST arguments*/
        if ((rc = read_post_vars(r, (const char **)&data)) != OK) {
            break;
        }

        /* Set Rexx stem array */
        parse_vars(r, data);

        /* Set Rexx Post data variable */
        SetRexxVar("WWWPOST_STRING", data);
        break;
    case (M_GET):
        /* point to GET arguments */
        data = r->args;

        /* Set Rexx stem array */
        parse_vars(r, data);
        break;
    default:
        break;
    }

    /* Set the return code */
    *(Retstr->strptr) = '\0';
    Retstr->strlength = 0;

//  modrexx_debug(r->server, "Entering WWWGetArgs routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWGetCookies                                      */
/*                                                                            */
/* Description: Rexx callable external function. Returns cookies.             */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWGetCookies(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWGetCookies(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    const char *data;
    char *val;
    char *name;
    char *pair;
    int num = 0;
    char varname [512];

    /* Check for valid arguments */
    if (Argc == 0) {
        return RXFUNC_BADCALL;
    }
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWGetCookies routine.");

    /* Set the return code */
    *(Retstr->strptr) = '\0';
    Retstr->strlength = 0;

    /* Initialize data */
    data = apr_table_get(r->headers_in, "Cookie");
    if (!data) {
        SetRexxVar("WWWCOOKIES.0", "0");
        return RXFUNC_OK;
    }

    /* Get the cookies out of the buffer */
    while (*data &&
           (pair = ap_getword(r->pool, (const char **)&data, ';'))) {
        if (*data == ' ') {
            ++data;
        }
        name = ap_getword(r->pool, (const char **)&pair, '=');
        while (*pair &&
               (val = ap_getword(r->pool, (const char **)&pair, '&'))) {
            ap_unescape_url(translate(val, "+", " "));

            /* We now have a name=value pair so create a Rexx variable */
            sprintf(varname, "WWWCOOKIES.%d.!NAME", ++num);
            SetRexxVar(varname, name);
            sprintf(varname, "WWWCOOKIES.%d.!VALUE", num);
            SetRexxVar(varname, val);
            sprintf(varname, "WWWCOOKIES.%s", name);
            SetRexxVar (varname, val);
        }
    }

    /* Now set the correct number of arguments */
    sprintf(varname, "%d", num);
    SetRexxVar("WWWCOOKIES.0", varname);

//  modrexx_debug(r->server, "Exiting WWWGetCookies routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSub_Req_Lookup_URI                              */
/*                                                                            */
/* Description: Rexx callable external function. Returns a subrequest handle. */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSub_Req_Lookup_URI(PSZ Name, LONG Argc, RXSTRING Argv[],
                                     PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSub_Req_Lookup_URI(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    request_rec *subreq;
    request_rec **rr;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWSub_Req_Lookup_URI routine.");

    /* Do the subrequest */
    subreq = ap_sub_req_lookup_uri(Argv[1].strptr, r, NULL);

    /* return the pointer to the subrequest record */
    rr = (request_rec **)Retstr->strptr;
    *(rr) = subreq;
    Retstr->strlength = sizeof(subreq);

//  modrexx_debug(r->server, "Exiting WWWSub_Req_Lookup_URI routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSub_Req_Lookup_File                             */
/*                                                                            */
/* Description: Rexx callable external function. Returns a subrequest handle. */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSub_Req_Lookup_File(PSZ Name, LONG Argc, RXSTRING Argv[],
                                      PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSub_Req_Lookup_File(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    request_rec *subreq;
    request_rec **rr;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWSub_Req_Lookup_File routine.");

    /* Do the subrequest */
    subreq = ap_sub_req_lookup_file(Argv[1].strptr, r, NULL);

    /* return the pointer to the subrequest record */
    rr = (request_rec **)Retstr->strptr;
    *(rr) = subreq;
    Retstr->strlength = sizeof(subreq);

//  modrexx_debug(r->server, "Exiting WWWSub_Req_Lookup_File routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWRun_Sub_Req                                     */
/*                                                                            */
/* Description: Rexx callable external function. Run the content handler for  */
/*              a subrequest.                                                 */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWRun_Sub_Req(PSZ Name, LONG Argc, RXSTRING Argv[],
                              PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWRun_Sub_Req(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    int rc;

    /* Check for valid arguments */
    if (Argc != 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWRun_Sub_Req routine.");

    /* Do the subrequest */
    rc = ap_run_sub_req(r);

    /* return the pointer to the subrequest record */
    sprintf(Retstr->strptr, "%d", rc);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWRun_Sub_Req routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWInternal_Redirect                               */
/*                                                                            */
/* Description: Rexx callable external function. Create a request from the    */
/*              URI argument and run it.                                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWInternal_Redirect(PSZ Name, LONG Argc, RXSTRING Argv[],
                                    PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWInternal_Redirect(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWInternal_Redirect routine.");

    /* Set the return code */
    *(Retstr->strptr) = '\0';
    Retstr->strlength = 0;

    /* Do the redirect */
    ap_internal_redirect(Argv[1].strptr, r);

//  modrexx_debug(r->server, "Exiting WWWInternal_Redirect routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWEscape_Path                                     */
/*                                                                            */
/* Description: Rexx callable external function. Converts a filesystem        */
/*              pathname and converts it into a properly escaped URI.         */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWEscape_Path(PSZ Name, LONG Argc, RXSTRING Argv[],
                              PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWEscape_Path(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    int partial;
    char *escurl;

    /* Check for valid arguments */
    if (Argc != 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1]) ||
        !RXVALIDSTRING(Argv[2])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWEscape_Path routine.");

    /* Perform the function */
    partial = atoi(Argv[2].strptr);
    escurl = ap_os_escape_path(r->pool, Argv[1].strptr, partial);

    /* Set the return value */
    if (strlen(escurl) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(escurl) + 1);
    }
    strcpy(Retstr->strptr, escurl);
    Retstr->strlength = strlen(escurl);

//  modrexx_debug(r->server, "Exiting WWWEscape_Path routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWConstruct_URL                                   */
/*                                                                            */
/* Description: Rexx callable external function. Constructs a fully qualified */
/*              url from the specified path.                                  */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWConstruct_URL(PSZ Name, LONG Argc, RXSTRING Argv[],
                                PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWConstruct_URL(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    char *url;

    /* Check for valid arguments */
    if (Argc != 2 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWConstruct_URL routine.");

    /* Perform the function */
    url = ap_construct_url(r->pool, Argv[1].strptr, r);

    /* Set the return value */
    if (strlen(url) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen (url) + 1);
    }
    strcpy(Retstr->strptr, url);
    Retstr->strlength = strlen(url);

//  modrexx_debug(r->server, "Exiting WWWConstruct_URL routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWHTTP_time                                       */
/*                                                                            */
/* Description: Rexx callable external function. Returns the time formatted   */
/*              per RFC 822 and 1123.                                         */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWHTTP_time(PSZ Name, LONG Argc, RXSTRING Argv[],
                            PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWHTTP_time(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    char *time_buf;

    /* Check for valid arguments */
    if (Argc != 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWHTTP_time routine.");

    /* Perform the function */
    time_buf = ap_ht_time(r->pool, apr_time_now(), "%a %d %b %Y %T %Z", 0);

    /* Set the return value */
    strcpy(Retstr->strptr, time_buf);
    Retstr->strlength = strlen(time_buf);

//  modrexx_debug(r->server, "Exiting WWWHTTP_time routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions support the Rexx Request_rec class declared in the */
/* file Apache.cls. This file can be included into an Object Rexx program via */
/* the ::REQUIRES directive.                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecConnection                                */
/*                                                                            */
/* Description: Rexx callable external function. Return the connection        */
/*              record pointer.                                               */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecConnection(PSZ Name, LONG Argc, RXSTRING Argv[],
                                   PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecConnection(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecConnection routine.");

    sprintf(Retstr->strptr, "%p", r->connection);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecConnection routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecServer                                    */
/*                                                                            */
/* Description: Rexx callable external function. Return the server            */
/*              record pointer.                                               */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecServer(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecServer(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecServer routine.");

    sprintf(Retstr->strptr, "%p", r->server);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecServer routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecNext                                      */
/*                                                                            */
/* Description: Rexx callable external function. Return the next              */
/*              record pointer.                                               */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecNext(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecNext(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecNext routine.");

    sprintf(Retstr->strptr, "%p", r->next);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecNext routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecPrev                                      */
/*                                                                            */
/* Description: Rexx callable external function. Return the prev              */
/*              record pointer.                                               */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecPrev(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecPrev(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecPrev routine.");

    sprintf(Retstr->strptr, "%p", r->prev);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecPrev routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecMain                                      */
/*                                                                            */
/* Description: Rexx callable external function. Return the main              */
/*              record pointer.                                               */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecMain(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecMain(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecMain routine.");

    sprintf(Retstr->strptr, "%p", r->main);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecMain routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecIsMain                                    */
/*                                                                            */
/* Description: Rexx callable external function. Returns true if this is the  */
/*              main request.                                                 */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecIsMain(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecIsMain(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecIsMain routine.");

    if (r->main == NULL) {
        strcpy(Retstr->strptr, "1");
    }
    else {
        strcpy(Retstr->strptr, "0");
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exitring WWWReqRecIsMain routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecThe_request                               */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              the_request field in the request record.                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecThe_request(PSZ Name, LONG Argc, RXSTRING Argv[],
                                    PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecThe_request(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecThe_request routine.");

    /* return the existing value */
    if (r->the_request != NULL &&
        strlen(r->the_request) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory (strlen (r->the_request) + 1);
    }
    if (r->the_request != NULL) {
        strcpy(Retstr->strptr, r->the_request);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecThe_request routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecProxyreq                                  */
/*                                                                            */
/* Description: Rexx callable external function. Return the true if this is   */
/*              a proxy request.                                              */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecProxyreq(PSZ Name, LONG Argc, RXSTRING Argv[],
                                 PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecProxyreq(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecProxyreq routine.");

    /* Return the existing value */
    sprintf(Retstr->strptr, "%d", r->proxyreq);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecProxyreq routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecHeader_only                               */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              header_only field in the request record.                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecHeader_only(PSZ Name, LONG Argc, RXSTRING Argv[],
                                    PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecHeader_only(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecHeader_only routine.");

    /* Return the existing value */
    sprintf(Retstr->strptr, "%d", r->header_only);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecHeader_only routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecProtocol                                  */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              protocol field in the request record.                         */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecProtocol(PSZ Name, LONG Argc, RXSTRING Argv[],
                                 PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecProtocol(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecProtocol routine.");

    /* return the existing value */
    if (r->protocol != NULL && strlen(r->protocol) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(r->protocol) + 1);
    }
    if (r->protocol != NULL) {
        strcpy(Retstr->strptr, r->protocol);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecProtocol routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecStatus_line                               */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              status_line field in the request record.                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New status_line value (optional)                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecStatus_line(PSZ Name, LONG Argc, RXSTRING Argv[],
                                    PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecStatus_line(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecStatus_line routine.");

    if (Argc == 2) {
        r->status_line = apr_pstrdup(r->pool, Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        if (r->status_line != NULL &&
            strlen(r->status_line) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory(strlen(r->status_line) + 1);
        }
        if (r->status_line != NULL) {
            strcpy(Retstr->strptr, r->status_line);
        }
        else {
            *(Retstr->strptr) = '\0';
        }
        }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecStatus_line routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecStatus                                    */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              status field in the request record.                           */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New status value (optional)                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecStatus(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecStatus(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecStatus routine.");

    if (Argc == 2) {
        r->status = atoi(Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        sprintf(Retstr->strptr, "%d", r->status);
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecStatus routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecMethod                                    */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              method field in the request record.                           */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New method name (optional)                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4 changes
ULONG APIENTRY WWWReqRecMethod(PSZ Name, LONG Argc, RXSTRING Argv[],
                               PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecMethod(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecMethod routine.");

    /* return the existing value */
    if (Argc == 2) {
        r->method = apr_pstrdup(r->pool, Argv[1].strptr);
    }
    if (r->method != NULL && strlen(r->method) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(r->method) + 1);
    }
    if (r->method != NULL) {
        strcpy(Retstr->strptr, r->method);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecMethod routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecMethod_number                             */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              method_number field in the request record.                    */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New method number (optional)                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecMethod_number(PSZ Name, LONG Argc, RXSTRING Argv[],
                                      PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecMethod_number(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecMethod_number routine.");

    /* return the existing value */
    if (Argc == 2) {
        r->method_number = atoi(Argv[1].strptr);
    }
    sprintf(Retstr->strptr, "%d", r->method_number);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecMethod_number routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecAllowed                                   */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              allowed field in the request record.                          */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New allowed value (optional)                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecAllowed(PSZ Name, LONG Argc, RXSTRING Argv[],
                                PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecAllowed(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecAllowed routine.");

    if (Argc == 2) {
        r->allowed = atoi(Argv[1].strptr);
    }
    /* return the value */
    sprintf(Retstr->strptr, "%d", (int)r->allowed);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecAllowed routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecBytes_sent                                */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              bytes_sent field in the request record.                       */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecBytes_sent(PSZ Name, LONG Argc, RXSTRING Argv[],
                                   PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecBytes_sent(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecBytes_sent routine.");

    /* return the existing value */
    sprintf(Retstr->strptr, "%d", (int)r->bytes_sent);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecBytes_sent routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecHeader_in                                 */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              headers_in field in the request record.                       */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Table lookup key string                                       */
/*              New value for the key (optional)                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecHeader_in(PSZ Name, LONG Argc, RXSTRING Argv[],
                                  PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecHeader_in(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    char *val;

    /* Check for valid arguments */
    if (Argc < 2) {
        return RXFUNC_BADCALL;
    }
    if (Argc > 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 3 && !RXVALIDSTRING(Argv[2])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecHeader_in routine.");

    if (Argc == 2) {
        /* return the value */
        val = (char *)apr_table_get(r->headers_in, Argv[1].strptr);
        if (val == NULL) {
            *(Retstr->strptr) = '\0';
        }
        else {
            if (strlen(val) > RXAUTOBUFLEN - 1) {
                Retstr->strptr = RexxAllocateMemory (strlen (val) + 1);
            }
            strcpy(Retstr->strptr, val);
        }
    }
    else {
        apr_table_set(r->headers_in, Argv[1].strptr, Argv[2].strptr);
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecHeader_in routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecHeader_out                                */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              headers_out field in the request record.                      */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Table lookup key string                                       */
/*              New value for the key                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecHeader_out(PSZ Name, LONG Argc, RXSTRING Argv[],
                                   PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecHeader_out(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc != 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1]) ||
        !RXVALIDSTRING(Argv[2])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecHeader_out routine.");

    apr_table_set(r->headers_out, Argv[1].strptr, Argv[2].strptr);
    *(Retstr->strptr) = '\0';
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecHeader_out routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecErr_header_out                            */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              err_headers_out field in the request record.                  */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Table lookup key string                                       */
/*              New value for the key (optional)                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecErr_header_out(PSZ Name, LONG Argc, RXSTRING Argv[],
                                       PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecErr_header_out(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    char *val;

    /* Check for valid arguments */
    if (Argc < 2) {
        return RXFUNC_BADCALL;
    }
    if (Argc > 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 3 && !RXVALIDSTRING(Argv[2])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecErr_header_out routine.");

    if (Argc == 2) {
        /* return the value */
        val = (char *)apr_table_get(r->err_headers_out, Argv[1].strptr);
        if (val == NULL) {
            *(Retstr->strptr) = '\0';
        }
        else {
            if (strlen(val) > RXAUTOBUFLEN - 1) {
                Retstr->strptr = RexxAllocateMemory (strlen (val) + 1);
            }
            strcpy(Retstr->strptr, val);
        }
    }
    else {
        apr_table_set(r->err_headers_out, Argv[1].strptr, Argv[2].strptr);
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecErr_header_out routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecSubprocess_env                            */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              subprocess_env field in the request record.                   */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Table lookup key string                                       */
/*              New value for the key (optional)                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecSubprocess_env(PSZ Name, LONG Argc, RXSTRING Argv[],
                                       PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecSubprocess_env(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    char *val;

    /* Check for valid arguments */
    if (Argc < 2) {
        return RXFUNC_BADCALL;
    }
    if (Argc > 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 3 && !RXVALIDSTRING(Argv[2])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecSubprocess_env routine.");

    if (Argc == 2) {
        /* return the value */
        val = (char *)apr_table_get(r->subprocess_env, Argv[1].strptr);
        if (val == NULL) {
            *(Retstr->strptr) = '\0';
        }
        else {
            if (strlen(val) > RXAUTOBUFLEN - 1) {
                Retstr->strptr = RexxAllocateMemory(strlen(val) + 1);
            }
            strcpy(Retstr->strptr, val);
        }
    }
    else {
        apr_table_set(r->subprocess_env, Argv[1].strptr, Argv[2].strptr);
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecSubprocess_env routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecNotes                                     */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              notes field in the request record.                            */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              Table lookup key string                                       */
/*              New value for the key (optional)                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecNotes(PSZ Name, LONG Argc, RXSTRING Argv[],
                              PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecNotes(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    char *val;

    /* Check for valid arguments */
    if (Argc < 2) {
        return RXFUNC_BADCALL;
    }
    if (Argc > 3 || !RXVALIDSTRING(Argv[0]) || !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 3 && !RXVALIDSTRING(Argv[2])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecNotes routine.");

    if (Argc == 2) {
        /* return the value */
        val = (char *)apr_table_get(r->notes, Argv[1].strptr);
        if (val == NULL) {
            *(Retstr->strptr) = '\0';
        }
        else {
            if (strlen(val) > RXAUTOBUFLEN - 1) {
                Retstr->strptr = RexxAllocateMemory(strlen(val) + 1);
            }
            strcpy(Retstr->strptr, val);
        }
    }
    else {
        apr_table_set(r->notes, Argv[1].strptr, Argv[2].strptr);
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecNotes routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecContent_type                              */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              content_type field in the request record.                     */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New content_type value (optional)                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecContent_type(PSZ Name, LONG Argc, RXSTRING Argv[],
                                     PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecContent_type(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecContent_type routine.");

    if (Argc == 2) {
        r->content_type = apr_pstrdup(r->pool, Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        if (r->content_type != NULL &&
            strlen(r->content_type) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory(strlen(r->content_type) + 1);
        }
        if (r->content_type != NULL) {
            strcpy(Retstr->strptr, r->content_type);
        }
        else {
            *(Retstr->strptr) = '\0';
        }
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecContent_type routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecContent_encoding                          */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              content_encoding field in the request record.                 */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New content_encoding value (optional)                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecContent_encoding(PSZ Name, LONG Argc, RXSTRING Argv[],
                                         PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecContent_encoding(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecContent_encoding routine.");

    if (Argc == 2) {
        r->content_encoding = apr_pstrdup(r->pool, Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        if (r->content_encoding != NULL &&
            strlen(r->content_encoding) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory(strlen(r->content_encoding) + 1);
        }
        if (r->content_encoding != NULL) {
            strcpy(Retstr->strptr, r->content_encoding);
        }
        else {
            *(Retstr->strptr) = '\0';
        }
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecContent_encoding routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecHandler                                   */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              handler field in the request record.                          */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New handler value (optional)                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecHandler(PSZ Name, LONG Argc, RXSTRING Argv[],
                                PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecHandler(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecHandler routine.");

    if (Argc == 2) {
        r->handler = apr_pstrdup(r->pool, Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        if (r->handler != NULL &&
            strlen(r->handler) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory(strlen(r->handler) + 1);
        }
        if (r->handler != NULL) {
            strcpy(Retstr->strptr, r->handler);
        }
        else {
            *(Retstr->strptr) = '\0';
        }
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecHandler routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecContent_languages                         */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              content_languages field in the request record.                */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New array member (optional)                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecContent_languages(PSZ Name, LONG Argc, RXSTRING Argv[],
                                          PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecContent_languages(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;
    char **element;
    char *list;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecContent_languages routine.");

    if (Argc == 2) {
        /* check to see if we need to create a new array */
        if (r->content_languages == NULL) {
            r->content_languages = apr_array_make(r->pool, 0,
                                                   sizeof(char *));
        }
        /* add the new array element */
        element = (char **)apr_array_push(r->content_languages);
        *element = apr_pstrdup(r->pool, RXSTRPTR(Argv[1]));
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing values */
        if (r->content_languages == NULL) {
            *(Retstr->strptr) = '\0';
        }
        else {
            list = apr_array_pstrcat(r->pool, r->content_languages, ',');
            if (strlen(list) > RXAUTOBUFLEN - 1) {
                Retstr->strptr = RexxAllocateMemory(strlen(list) + 1);
            }
            strcpy(Retstr->strptr, list);
        }
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecContent_languages routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecNo_cache                                  */
/*                                                                            */
/* Description: Rexx callable external function. Return the true if this is   */
/*              a no cache request.                                           */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New no_cache value                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecNo_cache(PSZ Name, LONG Argc, RXSTRING Argv[],
                                 PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecNo_cache(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc == 0) {
        return RXFUNC_BADCALL;
    }
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }
    if (Argc <= 2 && !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecNo_cache routine.");

    /* Set/return the existing value */
    if (Argc ==2) {
        r->no_cache = atoi(Argv[1].strptr);
    }
    sprintf(Retstr->strptr, "%d", r->no_cache);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecNo_cache routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecUri                                       */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              uri field in the request record.                              */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New uri value (optional)                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecUri(PSZ Name, LONG Argc, RXSTRING Argv[],
                            PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecUri(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecUri routine.");

    if (Argc == 2) {
        r->uri = apr_pstrdup(r->pool, Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        if (r->uri != NULL && strlen(r->uri) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory (strlen (r->uri) + 1);
        }
        if (r->uri != NULL) {
            strcpy(Retstr->strptr, r->uri);
        }
        else {
            *(Retstr->strptr) = '\0';
        }
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecUri routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecFilename                                  */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              filename field in the request record.                         */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New filename value (optional)                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecFilename(PSZ Name, LONG Argc, RXSTRING Argv[],
                                 PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecFilename(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecFilename routine.");

    if (Argc == 2) {
        r->filename = apr_pstrdup(r->pool, Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        if (r->filename != NULL &&
            strlen(r->filename) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory(strlen (r->filename) + 1);
        }
        if (r->filename != NULL) {
            strcpy(Retstr->strptr, r->filename);
        }
        else {
            *(Retstr->strptr) = '\0';
        }
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecFilename routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecPath_info                                 */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              path_info field in the request record.                        */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*              New path_info value (optional)                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecPath_info(PSZ Name, LONG Argc, RXSTRING Argv[],
                                  PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecPath_info(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec   * r;

    /* Check for valid arguments */
    if (Argc > 2 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }
    if (Argc == 2 && !RXVALIDSTRING(Argv[1])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecPath_info routine.");

    if (Argc == 2) {
        r->path_info = apr_pstrdup(r->pool, Argv[1].strptr);
        *(Retstr->strptr) = '\0';
    }
    else {
        /* return the existing value */
        if (r->path_info != NULL &&
            strlen(r->path_info) > RXAUTOBUFLEN - 1) {
            Retstr->strptr = RexxAllocateMemory (strlen (r->path_info) + 1);
        }
        if (r->path_info != NULL) {
            strcpy(Retstr->strptr, r->path_info);
        }
        else {
            *(Retstr->strptr) = '\0';
        }
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecPath_info routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecArgs                                      */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              args field in the request record.                             */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecArgs(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecArgs(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecArgs routine.");

    /* return the existing value */
    if (r->args != NULL && strlen(r->args) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(r->args) + 1);
    }
    if (r->args != NULL) {
        strcpy(Retstr->strptr, r->args);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecArgs routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecFinfo_stmode                              */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              finfo.st_mode field in the request record.                    */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecFinfo_stmode(PSZ Name, LONG Argc, RXSTRING Argv[],
                                     PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecFinfo_stmode(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecFinfo_stmode routine.");

    /* return the st_mode value */
    sprintf(Retstr->strptr, "%X", r->finfo.filetype);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecFinfo_stmode routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecUser                                      */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              user field in the connection record.                          */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecUser(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecUser(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecUser routine.");

    /* return the existing value */
    if (r->user != NULL && strlen(r->user) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(r->user) + 1);
    }
    if (r->user != NULL) {
        strcpy(Retstr->strptr, r->user);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecUser routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWReqRecAuth_type                                 */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              ap_auth_type field in the connection record.                  */
/*                                                                            */
/* Rexx Args:   Apache request handle                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWReqRecAuth_type(PSZ Name, LONG Argc, RXSTRING Argv[],
                                  PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWReqRecAuth_type(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    request_rec *r;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &r);

//  modrexx_debug(r->server, "Entering WWWReqRecAuth_type routine.");

    /* return the existing value */
    if (r->ap_auth_type != NULL &&
        strlen(r->ap_auth_type) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(r->ap_auth_type) + 1);
    }
    if (r->ap_auth_type != NULL) {
        strcpy(Retstr->strptr, r->ap_auth_type);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(r->server, "Exiting WWWReqRecAuth_type routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions support the Rexx Server_rec class declared in the  */
/* file Apache.cls. This file can be included into an Object Rexx program via */
/* the ::REQUIRES directive.                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSrvRecServer_admin                              */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              server_admin field in the server record.                      */
/*                                                                            */
/* Rexx Args:   Apache server handle                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSrvRecServer_admin(PSZ Name, LONG Argc, RXSTRING Argv[],
                                     PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSrvRecServer_admin(                              
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    server_rec *s;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &s);

//  modrexx_debug(s, "Entering WWWSrvRecServer_admin routine.");

    /* return the existing value */
    if (s->server_admin != NULL &&
        strlen(s->server_admin) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(s->server_admin) + 1);
    }
    if (s->server_admin != NULL) {
        strcpy(Retstr->strptr, s->server_admin);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(s, "Exiting WWWSrvRecServer_admin routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSrvRecServer_hostname                           */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              server_hostname field in the server record.                   */
/*                                                                            */
/* Rexx Args:   Apache server handle                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSrvRecServer_hostname(PSZ Name, LONG Argc, RXSTRING Argv[],
                                        PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSrvRecServer_hostname(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    server_rec *s;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &s);

//  modrexx_debug(s, "Entering WWWSrvRecServer_hostname routine.");

    /* return the existing value */
    if (s->server_hostname != NULL &&
        strlen(s->server_hostname) > RXAUTOBUFLEN - 1) {
        Retstr->strptr = RexxAllocateMemory(strlen(s->server_hostname) + 1);
    }
    if (s->server_hostname != NULL) {
        strcpy(Retstr->strptr, s->server_hostname);
    }
    else {
        *(Retstr->strptr) = '\0';
    }
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(s, "Exiting WWWSrvRecServer_hostname routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSrvRecPort                                      */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              port field in the server record.                              */
/*                                                                            */
/* Rexx Args:   Apache server handle                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSrvRecPort(PSZ Name, LONG Argc, RXSTRING Argv[],
                             PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSrvRecPort(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    server_rec *s;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &s);

//  modrexx_debug(s, "Entering WWWSrvRecPort routine.");

    /* return the existing value */
    sprintf(Retstr->strptr, "%d", (int)s->port);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(s, "Exiting WWWSrvRecPort routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWSrvRecIs_virtual                                */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              is_virtual field in the server record.                        */
/*                                                                            */
/* Rexx Args:   Apache server handle                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWSrvRecIs_virtual(PSZ Name, LONG Argc, RXSTRING Argv[],
                                   PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWSrvRecIs_virtual(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)
{
    server_rec *s;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &s);

//  modrexx_debug(s, "Entering WWWSrvRecIs_virtual routine.");

    /* return the existing value */
    sprintf(Retstr->strptr, "%d", s->is_virtual);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(s, "Exiting WWWSrvRecIs_virtual routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions support the Rexx Connection_rec class declared in  */
/* the file Apache.cls. This file can be included into an Object Rexx program */
/* via the ::REQUIRES directive.                                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx External Function: WWWCnxRecAborted                                   */
/*                                                                            */
/* Description: Rexx callable external function. Give direct access to the    */
/*              aborted field in the connection record.                       */
/*                                                                            */
/* Rexx Args:   Apache connx handle                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TPG 2.4
ULONG APIENTRY WWWCnxRecAborted(PSZ Name, LONG Argc, RXSTRING Argv[],
                                PSZ Queuename, PRXSTRING Retstr)
                                 */
APIRET APIENTRY WWWCnxRecAborted(
     const char* Name,
     size_t Argc,
     RXSTRING Argv[],
     const char* Queuename,
     PRXSTRING Retstr)

{
    conn_rec *c;

    /* Check for valid arguments */
    if (Argc > 1 || !RXVALIDSTRING(Argv[0])) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &c);

//  modrexx_debug(c->base_server, "Entering WWWCnxRecAborted routine.");

    /* return the existing value */
    sprintf(Retstr->strptr, "%d", c->aborted);
    Retstr->strlength = strlen(Retstr->strptr);

//  modrexx_debug(c->base_server, "Exiting WWWCnxRecAborted routine.");

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    reg_rexx_ext_funcs                                            */
/*                                                                            */
/* Description: Registers all of the Rexx external functions declared above.  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

void reg_rexx_ext_funcs (void)
{

    /* Register the Rexx HTTPD external functions */

    /* These are the generic external functions */
    RexxRegisterFunctionExe ("WWWGetVersion",  (PFN) WWWGetVersion);
    RexxRegisterFunctionExe ("WWWSendHTTPHeader",  (PFN) WWWSendHTTPHeader);
    RexxRegisterFunctionExe ("WWWAddCookie",  (PFN) WWWAddCookie);
    RexxRegisterFunctionExe ("WWWSetHeaderValue",  (PFN) WWWSetHeaderValue);
    RexxRegisterFunctionExe ("WWWLogError",  (PFN) WWWLogError);
    RexxRegisterFunctionExe ("WWWLogWarning",  (PFN) WWWLogWarning);
    RexxRegisterFunctionExe ("WWWLogInfo",  (PFN) WWWLogInfo);
    RexxRegisterFunctionExe ("WWWLogReason",  (PFN) WWWLogReason);
    RexxRegisterFunctionExe ("WWWGetArgs",  (PFN) WWWGetArgs);
    RexxRegisterFunctionExe ("WWWGetCookies",  (PFN) WWWGetCookies);
    RexxRegisterFunctionExe ("WWWSub_Req_Lookup_URI", (PFN) WWWSub_Req_Lookup_URI);
    RexxRegisterFunctionExe ("WWWSub_Req_Lookup_File", (PFN) WWWSub_Req_Lookup_File);          
    RexxRegisterFunctionExe ("WWWRun_Sub_Req",  (PFN) WWWRun_Sub_Req);
    RexxRegisterFunctionExe ("WWWInternal_Redirect", (PFN) WWWInternal_Redirect);
    RexxRegisterFunctionExe ("WWWEscape_Path",  (PFN) WWWEscape_Path);
    RexxRegisterFunctionExe ("WWWConstruct_URL",  (PFN) WWWConstruct_URL);
    RexxRegisterFunctionExe ("WWWHTTP_time",  (PFN) WWWHTTP_time);

    /* These are the oo request record external functions */
    RexxRegisterFunctionExe ("WWWReqRecConnection",  (PFN) WWWReqRecConnection);
    RexxRegisterFunctionExe ("WWWReqRecNext",  (PFN) WWWReqRecNext);
    RexxRegisterFunctionExe ("WWWReqRecPrev",  (PFN) WWWReqRecPrev);
    RexxRegisterFunctionExe ("WWWReqRecMain",  (PFN) WWWReqRecMain);
    RexxRegisterFunctionExe ("WWWReqRecIsMain",  (PFN) WWWReqRecIsMain);
    RexxRegisterFunctionExe ("WWWReqRecThe_request", (PFN) WWWReqRecThe_request);
    RexxRegisterFunctionExe ("WWWReqRecProxyreq",  (PFN) WWWReqRecProxyreq);
    RexxRegisterFunctionExe ("WWWReqRecServer",  (PFN) WWWReqRecServer);
    RexxRegisterFunctionExe ("WWWReqRecHeader_only", (PFN) WWWReqRecHeader_only);
    RexxRegisterFunctionExe ("WWWReqRecProtocol",  (PFN) WWWReqRecProtocol);
    RexxRegisterFunctionExe ("WWWReqRecStatus_line", (PFN) WWWReqRecStatus_line);
    RexxRegisterFunctionExe ("WWWReqRecStatus",  (PFN) WWWReqRecStatus);
    RexxRegisterFunctionExe ("WWWReqRecMethod",  (PFN) WWWReqRecMethod);
    RexxRegisterFunctionExe ("WWWReqRecMethod_number", (PFN) WWWReqRecMethod_number);
    RexxRegisterFunctionExe ("WWWReqRecAllowed",  (PFN) WWWReqRecAllowed);
    RexxRegisterFunctionExe ("WWWReqRecBytes_sent",  (PFN) WWWReqRecBytes_sent);
    RexxRegisterFunctionExe ("WWWReqRecHeader_in",  (PFN) WWWReqRecHeader_in);
    RexxRegisterFunctionExe ("WWWReqRecHeader_out",  (PFN) WWWReqRecHeader_out);
    RexxRegisterFunctionExe ("WWWReqRecErr_header_out", (PFN) WWWReqRecErr_header_out);
    RexxRegisterFunctionExe ("WWWReqRecSubprocess_env", (PFN) WWWReqRecSubprocess_env);
    RexxRegisterFunctionExe ("WWWReqRecNotes",  (PFN) WWWReqRecNotes);
    RexxRegisterFunctionExe ("WWWReqRecContent_type", (PFN) WWWReqRecContent_type);
    RexxRegisterFunctionExe ("WWWReqRecContent_encoding", (PFN) WWWReqRecContent_encoding);
    RexxRegisterFunctionExe ("WWWReqRecHandler",  (PFN) WWWReqRecHandler);
    RexxRegisterFunctionExe ("WWWReqRecContent_languages", (PFN) WWWReqRecContent_languages);
    RexxRegisterFunctionExe ("WWWReqRecNo_cache", (PFN) WWWReqRecNo_cache);
    RexxRegisterFunctionExe ("WWWReqRecUri", (PFN) WWWReqRecUri);
    RexxRegisterFunctionExe ("WWWReqRecFilename",  (PFN) WWWReqRecFilename);
    RexxRegisterFunctionExe ("WWWReqRecPath_info",  (PFN) WWWReqRecPath_info);
    RexxRegisterFunctionExe ("WWWReqRecArgs",  (PFN) WWWReqRecArgs);
    RexxRegisterFunctionExe ("WWWReqRecFinfo_stmode", (PFN) WWWReqRecFinfo_stmode);
    RexxRegisterFunctionExe ("WWWReqRecUser",  (PFN) WWWReqRecUser);
    RexxRegisterFunctionExe ("WWWReqRecAuth_type",  (PFN) WWWReqRecAuth_type);

    /* These are the oo server record external functions */
    RexxRegisterFunctionExe ("WWWSrvRecServer_admin", (PFN) WWWSrvRecServer_admin);
    RexxRegisterFunctionExe ("WWWSrvRecServer_hostname", (PFN) WWWSrvRecServer_hostname);
    RexxRegisterFunctionExe ("WWWSrvRecPort",  (PFN) WWWSrvRecPort);
    RexxRegisterFunctionExe ("WWWSrvRecIs_virtual",  (PFN) WWWSrvRecIs_virtual);

    /* These are the oo connection record external functions */
    RexxRegisterFunctionExe ("WWWCnxRecAborted",  (PFN) WWWCnxRecAborted);

    return;
}

