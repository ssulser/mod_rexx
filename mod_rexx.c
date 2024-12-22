/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      Mod_Rexx.c                                                    */
/*                                                                            */
/* Author:      The Programmers' Guild, Inc.                                  */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Apache module to support Rexx/Object Rexx.                    */
/*                                                                            */
/* Copyright (C) The Programmers' Guild, 2021. All Rights Reserved.           */
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
/* Data declarations.                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

const char *version = MOD_REXX_VERSION;
const char *compDate = __DATE__;
const char *compTime = __TIME__;

static const char *lf = "\n";

/* This table is used to hold request record pointers. Each request record    */
/* pointer is stored in text form and is indexed by the owner thread ID. This */
/* ensures that the correct request record will always be used by the Rexx    */
/* exit routines.                                                             */
apr_table_t *mod_rexx_global_table;


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    CreateTempFile                                                */
/*                                                                            */
/* Description: Create a temporary file.                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

const char * CreateTempFile(request_rec *r, char *fntemplate)
{
    char * TempName;
    int i, j;
    unsigned int fno;
    char *rbuf;
    char templatechar = '?';
    int len;
    apr_file_t *fh;

    /* get a random number */
    srand( (unsigned)time(NULL));
    fno = (unsigned int)rand();

    /* copy file name to our buffer */
    TempName = apr_pstrdup(r->pool, fntemplate);

    /* substitute pattern in file name and try to open the file */
    do {
        rbuf = apr_psprintf(r->pool, "%05lu", (unsigned long)fno);
        len = strlen(rbuf);
        for (i = 0, j = len - 1; i < strlen(fntemplate) && j > (len - 6); i++) {
            if (fntemplate[i] == templatechar) {
                TempName[i] = rbuf[j--];
            }
        }
        fno++;
    } while (apr_file_open(&fh, TempName, APR_WRITE | APR_CREATE | APR_EXCL,
             APR_OS_DEFAULT, r->pool) != APR_SUCCESS);

    /* now close the opened file so it can be reopened by Rexx */
    apr_file_close(fh);

    return(TempName);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    SetRexxVar                                                    */
/*                                                                            */
/* Description: Set a Rexx variable.                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int SetRexxVar(const char * varname, char * value)
{
    SHVBLOCK   block;
    int rc;

    block.shvcode = RXSHV_SET;
    block.shvret = (u_char) 0;
    block.shvnext = NULL;

    MAKERXSTRING(block.shvname, varname, strlen(varname));
    block.shvnamelen = strlen(varname);

    /* The possibility exists that we will be passed a NULL ptr for the value */
    /* of the Rexx variable so take care of that possibility when setting up  */
    /* the shvvalue and shvvaluelen variables.                                */
    if (value == NULL) {
        MAKERXSTRING(block.shvvalue, "\0", 0);
        block.shvvaluelen = 0;
    }
    else {
        MAKERXSTRING(block.shvvalue, value, strlen(value));
        block.shvvaluelen = strlen(value);
    }

    rc = RexxVariablePool(&block);
    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    GetReqRecPtr                                                  */
/*                                                                            */
/* Description: Get the current thread's request record pointer from the      */
/*              global table.                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

request_rec *GetReqRecPtr(void)
{
   request_rec *r;
   apr_os_thread_t os_tid;
   char os_tid_str[64];
   const char *r_str;

   /* Get pointer from the global table */
   os_tid = apr_os_thread_current();
   sprintf(os_tid_str, "%p", (void *)os_tid);
   r_str = apr_table_get(mod_rexx_global_table, os_tid_str);

   /* make sure we found a value in the table */
   if (r_str == NULL || *r_str == '\0') {
       return NULL;
   }

   /* return the pointer */
   sscanf(r_str, "%p", &r);
   return (r);
   }


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    Rexx_IO_Exit                                                  */
/*                                                                            */
/* Description: I/O exit for the Rexx procedure.                              */
/*                                                                            */
/* Notes: Do NOT use the user area to store the Apache request record         */
/*        pointer. Doing so is a bug in a multi-threaded environment (the     */
/*        userarea is common to all threads)                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

LONG APIENTRY Rexx_IO_Exit(LONG ExitNumber, LONG Subfunction,
                           PEXIT ParmBlock)
{
    request_rec *r;
    RXSIOSAY_PARM *sparm;

    /* get the thread's request record pointer */
    r = GetReqRecPtr();
    if (r == NULL) {
        /* Just ignore the I/O if we can't find a request record pointer   */
        /* This is probably because the user is attempting I/O from a Rexx */
        /* thread we don't know about.                                     */
        return RXEXIT_HANDLED;
    }

    /* Perform the exit function */
    switch (Subfunction) {
    case RXSIOSAY:
        sparm = (RXSIOSAY_PARM *)ParmBlock;
        ap_rputs(sparm->rxsio_string.strptr, r);
        ap_rputs(lf, r);
        return RXEXIT_HANDLED;
    case RXSIOTRC:
        sparm = (RXSIOSAY_PARM *)ParmBlock;
        ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0, r,
                  "%s", sparm->rxsio_string.strptr);
        return RXEXIT_HANDLED;
    case RXSIOTRD:
    case RXSIODTR:
        break;
    default:
        break;
    }

    return RXEXIT_RAISE_ERROR;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    Rexx_IO_Exit_2                                                */
/*                                                                            */
/* Description: I/O exit for non-content Rexx handler procedures.             */
/*                                                                            */
/* Notes: Do NOT use the user area to store the Apache request record         */
/*        pointer. Doing so is a bug in a multi-threaded environment (the     */
/*        userarea is common to all threads)                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

LONG APIENTRY Rexx_IO_Exit_2(LONG ExitNumber, LONG Subfunction,
                             PEXIT ParmBlock)
{
    request_rec *r;
    RXSIOSAY_PARM *sparm;

    /* get the thread's request record pointer */
    r = GetReqRecPtr();
    if (r == NULL) {
        /* Just ignore the I/O if we can't find a request record pointer   */
        /* This is probably because the user is attempting I/O from a Rexx */
        /* thread we don't know about.                                     */
        return RXEXIT_HANDLED;
    }

    /* Perform the exit function */
    switch (Subfunction) {
    case RXSIOSAY:
        /* Do not allow any output */
        return (RXEXIT_HANDLED);
    case RXSIOTRC:
        sparm = (RXSIOSAY_PARM *)ParmBlock;
        ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0, r,
                  "%s", sparm->rxsio_string.strptr);
        return (RXEXIT_HANDLED);
    case RXSIOTRD:
    case RXSIODTR:
        /* Do not allow any input or output */
        return (RXEXIT_HANDLED);
    default:
       break;
    }

    return RXEXIT_RAISE_ERROR;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    Rexx_INI_Exit                                                 */
/*                                                                            */
/* Description: INI exit for the Rexx procedure.                              */
/*                                                                            */
/* Notes: Do NOT use the user area to store the Apache request record         */
/*        pointer. Doing so is a bug in a multi-threaded environment (the     */
/*        userarea is common to all threads)                                  */
/*                                                                            */
/*        Do NOT try to read the POST name/value pairs from the browser       */
/*        client in this exit. Really bad things will happen.                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* before change to new api
int Rexx_INI_Exit(int ExitNumber, int Subfunction,
                            PEXIT ParmBlock)
 */
LONG APIENTRY Rexx_INI_Exit(LONG ExitNumber,
                                       LONG Subfunction,
                                       PEXIT ParmbBlock)
{
    request_rec *r;
    request_rec *subr;
    rexx_var_rec *sr;
    char *varname;
    rexx_config *c;

    /* get the thread's request record pointer, this should always succeed */
    r = GetReqRecPtr();

    /* Get the Rexx config info */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &rexx_module);

    /* Perform the exit function */
    switch (Subfunction) {
    case RXINIEXT:
//      modrexx_debug(r->server, "Entering Rexx_INI_Exit routine.");

        /* Register all our Rexx external functions */
        reg_rexx_ext_funcs();

        /* Set our standard CGI Rexx variables */
        SetRexxVar("WWWAUTH_TYPE", r->ap_auth_type);
        SetRexxVar("WWWCONTENT_LENGTH",
                          (char *) apr_table_get(r->headers_in, "Content-Length"));
        SetRexxVar("WWWCONTENT_TYPE",
                          (char *) apr_table_get(r->headers_in, "Content-Type"));
        SetRexxVar("WWWGATEWAY_INTERFACE", (char *)version);
        SetRexxVar("WWWHTTP_USER_ACCEPT",
                          (char *) apr_table_get(r->headers_in, "Accept"));
        SetRexxVar("WWWHTTP_USER_AGENT",
                          (char *) apr_table_get(r->headers_in, "User-Agent"));
        SetRexxVar("WWWPATH_INFO", r->path_info);

        subr = ap_sub_req_lookup_uri(r->path_info, r, NULL);
        SetRexxVar("WWWPATH_TRANSLATED", subr->filename);
        ap_destroy_sub_req(subr);

        SetRexxVar("WWWQUERY_STRING", r->args);
    /*  SetRexxVar("WWWREMOTE_ADDR", r->connection->remote_ip); TPG 2.4 */
        SetRexxVar("WWWREMOTE_ADDR", r->useragent_ip);
        SetRexxVar("WWWREMOTE_HOST",
                                  (char *) ap_get_remote_host(r->connection,
                                           r->per_dir_config,
                                           REMOTE_HOST, NULL));
        SetRexxVar("WWWREMOTE_IDENT", (char *) ap_get_remote_logname(r));
        SetRexxVar("WWWREMOTE_USER", r->user);
        SetRexxVar("WWWREQUEST_METHOD", (char *) r->method);
        SetRexxVar("WWWSCRIPT_NAME", r->uri);
        SetRexxVar("WWWSERVER_NAME", (char *) ap_get_server_name(r));

        varname = apr_psprintf(r->pool, "%d", ap_get_server_port(r));
        SetRexxVar("WWWSERVER_PORT", varname);

        SetRexxVar("WWWSERVER_PROTOCOL", r->protocol);
        SetRexxVar("WWWSERVER_SOFTWARE", (char *) ap_get_server_description());

        /* Define some other useful variables to the Rexx program. */
        SetRexxVar("WWWDEFAULT_TYPE", "text/plain");
        if (r->main == NULL) {
            SetRexxVar("WWWIS_MAIN_REQUEST", "1");
        }
        else {
            SetRexxVar("WWWIS_MAIN_REQUEST", "0");
        }
        SetRexxVar("WWWSERVER_ROOT",
                   ap_server_root_relative (r->pool, ""));
        SetRexxVar("WWWUNPARSEDURI", r->unparsed_uri);
        SetRexxVar("WWWURI", r->uri);
        SetRexxVar("WWWFILENAME", r->filename);
        SetRexxVar("WWWPOST_STRING", NULL);

        /* Define the Rexx variables defined in the Apache httpd.conf file.   */
        sr = c->rexxvars;
        while (sr != NULL) {
            SetRexxVar(sr->rexx_var_name, sr->rexx_var_value);
            sr = sr->next;
        }

        /* Define the Rexx RSP compiler information variables. */
        SetRexxVar("WWWRSPCOMPILER", c->rspcompiler);
        SetRexxVar("WWWFNAMETEMPLATE", c->fnametemplate);

//      modrexx_debug(r->server, "Exiting Rexx_INI_Exit routine.");

        return RXEXIT_HANDLED;
    default:
        break;
    }

    return RXEXIT_RAISE_ERROR;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    Call_Rexx                                                     */
/*                                                                            */
/* Description: Call the Rexx interpreter from an Apache handler.             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int Call_Rexx(request_rec *r, char *rxprocpath, char *rxarg, char *io_exit,
               int *rxrc)
{
    RXSTRING argv[1];
    RXSTRING retstr;
    short src;
    char return_buffer[250];
    RXSYSEXIT sysexit[3];
    rexx_config *c;
    APIRET rc;
    apr_os_thread_t os_tid;
    char os_tid_str[64];
    char r_str[64];
    static int exits_reg = FALSE; // boolean for registering Rexx exits

//  modrexx_debug(r->server, "Entering Call_Rexx function.");

    /* Get the Rexx config userarea info */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &rexx_module);

    /* Setup so the Rexx exits can retrieve the request_rec */
    /* from the global table using the thread ID as a key.  */
    os_tid = apr_os_thread_current();
    sprintf(os_tid_str, "%p", (void *)os_tid);
    sprintf(r_str, "%p", r);
    apr_table_set(mod_rexx_global_table, os_tid_str, r_str);

    /* Set up the Rexx exits */
    if (exits_reg == FALSE) {
        rc = RexxRegisterExitExe(WWW_INI_EXIT,  Rexx_INI_Exit, NULL);
        if (rc) {
            ap_log_error(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0,
                         r->server, "Error %d registering Rexx WWW_INI_EXIT",
                         (int)rc);
        }
        rc = RexxRegisterExitExe(WWW_IO_EXIT,  Rexx_IO_Exit, NULL);
        if (rc) {
            ap_log_error(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0,
                         r->server, "Error %d registering Rexx WWW_IO_EXIT",
                         (int)rc);
        }
        rc = RexxRegisterExitExe(WWW_IO_EXIT_2,  Rexx_IO_Exit_2, NULL);
        if (rc) {
            ap_log_error(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0,
                         r->server, "Error %d registering Rexx WWW_IO_EXIT_2",
                         (int)rc);
        }
        exits_reg = TRUE;
    }
    sysexit[0].sysexit_name = io_exit;
    sysexit[0].sysexit_code = RXSIO;
    sysexit[1].sysexit_name = WWW_INI_EXIT;
    sysexit[1].sysexit_code = RXINI;
    sysexit[2].sysexit_name = NULL;
    sysexit[2].sysexit_code = RXENDLST;

    /* Call the Rexx interpreter */
    MAKERXSTRING(argv[0], rxarg, strlen(rxarg));
    MAKERXSTRING(retstr, return_buffer, sizeof(return_buffer));
//  modrexx_debug(r->server, "Calling Rexx interpreter.");
    *rxrc = RexxStart((size_t) 1, (PRXSTRING) argv,  (const char*) rxprocpath, NULL, RX_ADDR_ENV, RXCOMMAND,
                      sysexit, &src, &retstr);
//  modrexx_debug(r->server, "Returned from Rexx interpreter.");
    #ifdef HAVE_RexxWaitForTermination
    RexxWaitForTermination();
    #endif

    /* test Rexx return string and set the return code if necessary */
    if (RXNULLSTRING(retstr) || RXZEROLENSTRING(retstr)) {
        src = 0;
    }

    /* release Rexx allocated storage if necessary */
    if (RXSTRPTR(retstr) != return_buffer) {
        RexxFreeMemory(RXSTRPTR(retstr));
    }

    /* Cleanup */
    return (int) src;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    get_rexx_err_str                                              */
/*                                                                            */
/* Description: Return the Rexx error string.                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

char *get_rexx_err_str(request_rec *r, int rxrc)
{
    char *errstr;

    switch (rxrc) {
    case -3:
        errstr = "Error 3  Failure during initialization. "
         "Program may be missing or unreadable.";
        break;
    case -4:
        errstr = "Error 4  Program interrupted.";
        break;
    case -5:
        errstr = "Error 5  System resources exhausted.";
        break;
    case -6:
        errstr = "Error 6  Unmatched \"/*\" or quote.";
        break;
    case -7:
        errstr = "Error 7  WHEN or OTHERWISE expected.";
        break;
    case -8:
        errstr = "Error 8  Unexpected THEN or ELSE.";
        break;
    case -9:
        errstr = "Error 9  Unexpected WHEN or OTHERWISE.";
        break;
    case -10:
        errstr = "Error 10  Uexpected or unmatched END.";
        break;
    case -11:
        errstr = "Error 11  Control Stack full.";
        break;
    case -13:
        errstr = "Error 13  Invalid character in program.";
        break;
    case -14:
        errstr = "Error 14  Incomplete DO/SELECT/IF.";
        break;
    case -15:
        errstr = "Error 15  Invalid hexadecimal or binary string.";
        break;
    case -16:
        errstr = "Error 16  Label not found.";
        break;
    case -17:
        errstr = "Error 17  Unexpected PROCEDURE.";
        break;
    case -18:
        errstr = "Error 18  THEN expected.";
        break;
    case -19:
        errstr = "Error 19  String or symbol expected.";
        break;
    case -20:
        errstr = "Error 20  Symbol expected.";
        break;
    case -21:
        errstr = "Error 21  Invalid data on end of clause.";
        break;
    case -22:
        errstr = "Error 22  Invalid character string.";
        break;
    case -23:
        errstr = "Error 23  Invalid data string.";
        break;
    case -24:
        errstr = "Error 24  Invalid TRACE request.";
        break;
    case -25:
        errstr = "Error 25  Invalid subkeyword found.";
        break;
    case -26:
        errstr = "Error 26  Invalid whole number.";
        break;
    case -27:
        errstr = "Error 27  Invalid DO syntax.";
        break;
    case -28:
        errstr = "Error 28  Invalid LEAVE or ITERATE.";
        break;
    case -29:
        errstr = "Error 29  Environment name too long.";
        break;
    case -30:
        errstr = "Error 30  Name or string too long.";
        break;
    case -31:
        errstr = "Error 31  Name starts with number or \".\".";
        break;
    case -33:
        errstr = "Error 33  Invalid expression result.";
        break;
    case -34:
        errstr = "Error 34  Logical value not 0 or 1.";
        break;
    case -35:
        errstr = "Error 35  Invalid expression.";
        break;
    case -36:
        errstr = "Error 36  Unmatched \"(\" or \"[\" in expression.";
        break;
    case -37:
        errstr = "Error 37  Unexpected \",\", \")\", or \"]\".";
        break;
    case -38:
        errstr = "Error 38  Invalid template or pattern.";
        break;
    case -40:
        errstr = "Error 40  Incorrect call to routine.";
        break;
    case -41:
        errstr = "Error 41  Bad arithmetic conversion.";
        break;
    case -42:
        errstr = "Error 42  Arithmetic overflow/underflow.";
        break;
    case -43:
        errstr = "Error 43  Routine not found.";
        break;
    case -44:
        errstr = "Error 44  Function or message did not return data.";
        break;
    case -45:
        errstr = "Error 45  No data specified on function RETURN.";
        break;
    case -46:
        errstr = "Error 46  Invalid variable reference.";
        break;
    case -47:
        errstr = "Error 47  Unexpected lable.";
        break;
    case -48:
        errstr = "Error 48  Failure in system service.";
        break;
    case -49:
        errstr = "Error 49  Interpretation error.";
        break;
    case -90:
        errstr = "Error 90  External name not found.";
        break;
    case -91:
        errstr = "Error 91  No result object.";
        break;
    case -93:
        errstr = "Error 93  Incorrect call to method.";
        break;
    case -97:
        errstr = "Error 97  Object method not found.";
        break;
    case -98:
        errstr = "Error 98  Execution error.";
        break;
    case -99:
        errstr = "Error 99  Translation error.";
        break;
    default:
        errstr = apr_psprintf(r->pool,
                         "Unknown error %d returned from the Rexx Interpreter.",
                         (int)rxrc);
        break;
    }

    return errstr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexxstart_error_processor                                     */
/*                                                                            */
/* Description: Output a page to the browser showing the error code and       */
/*              message from the Rexx Interpreter.                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

void rexxstart_error_processor(request_rec *r, char *rxprocpath,
                               int rxrc)
{
    char *outstr;
    char *errstr;

    errstr = get_rexx_err_str(r, rxrc);
    if (rxrc < 0) {
        outstr = apr_psprintf(r->pool,
                 "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
                 "<html><head>\n"
                 "<title>Internal Server Error</title>\n"
                 "</head></body>\n"
                 "<h1>Internal Server Error</h1>\n"
                 "<h2>Rexx Interpreter Error</h2>\n"
                 "<p>Rexx procedure filename: %s\n"
                 "<br />%s</p>\n"
                 "<hr />\n"
                 "<address>%s at %s Port %d</address>\n"
                 "</body></html>\n",
                rxprocpath, errstr, ap_get_server_description(),
                ap_get_server_name(r), ap_get_server_port(r));
    }
    else {
        outstr = apr_psprintf(r->pool,
                 "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
                 "<html><head>\n"
                 "<title>Internal Server Error</title>\n"
                 "</head></body>\n"
                 "<h1>Internal Server Error</h1>\n"
                 "<h2>System Error Returned from the Rexx Interpreter</h2>\n"
                 "<p>Rexx procedure filename: %s\n"
                 "<br />%s</p>\n"
                 "<hr />\n"
                 "<address>%s at %s Port %d</address>\n"
                "</body></html>\n",
                rxprocpath, errstr, ap_get_server_description(),
                ap_get_server_name(r), ap_get_server_port(r));
    }
    ap_custom_response(r,HTTP_INTERNAL_SERVER_ERROR, outstr);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
    modrexx_error(r->server, rxprocpath);
    modrexx_error(r->server, errstr);
#pragma GCC diagnostic pop
    return;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    rexx_register_hooks                                           */
/*                                                                            */
/* Description: This handler is invoked to register all the standard handler  */
/*              hooks.                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static void rexx_register_hooks(apr_pool_t *p)
{

    ap_hook_post_config(rexx_post_config_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_translate_name(rexx_translation_handler, NULL, NULL,
                           APR_HOOK_MIDDLE);
    ap_hook_auth_checker(rexx_authentication_handler, NULL, NULL,
                         APR_HOOK_MIDDLE);
    ap_hook_check_user_id(rexx_authorization_handler, NULL, NULL,
                          APR_HOOK_MIDDLE);
    ap_hook_access_checker(rexx_access_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_type_checker(rexx_mime_type_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_fixups(rexx_fixup_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_log_transaction(rexx_logging_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_header_parser(rexx_header_parser_handler, NULL, NULL,
                          APR_HOOK_MIDDLE);
    ap_hook_post_read_request(rexx_post_request_handler, NULL, NULL,
                              APR_HOOK_MIDDLE);
    ap_hook_handler(rexx_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(rexx_rsphandler, NULL, NULL, APR_HOOK_MIDDLE);
};


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The list of callback routines and data structures that provide the hooks   */
/* into our module from the other parts of the Apache server.                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

module AP_MODULE_DECLARE_DATA rexx_module =
{
    STANDARD20_MODULE_STUFF,
    create_rexx_dir_config,            /* per-directory config creator        */
    NULL,                              /* dir config merger                   */
    NULL,                              /* server config creator               */
    NULL,                              /* server config merger                */
    rexx_cmds,                         /* command table                       */
    rexx_register_hooks                /* register hooks                      */
};
