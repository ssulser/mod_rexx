/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test2rex                                                      */
/*                                                                            */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Test page for the Mod_Rexx Apache module (serialization test).*/
/*                                                                            */
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
/*----------------------------------------------------------------------------*/


call rxfuncadd 'SysLoadFuncs', 'rexxutil', 'SysLoadFuncs'
call SysLoadFuncs

/* version of this script */
script_version = 'v2.2.0'

call rxfuncadd 'SysLoadFuncs', 'rexxutil', 'SysLoadFuncs'
call SysLoadFuncs

/* these are some typical Apache return codes */
DECLINED  = -1    /* Module declines to handle */
DONE      = -2    /* Module has served the response completely */
OK        = 0     /* Module has handled this stage. */
NOT_FOUND = 404   /* Main document not found. */

/* get environment stuff */
parse source env . path
if env='OS/2' then envname='OS2ENVIRONMENT'
else envname='ENVIRONMENT'
parse upper version v

/* get the Apache request record ptr */
r = arg(1)

/* set content-type and send the HTTP header */
call WWWSendHTTPHeader r, "text/html" /* note: this call is NOT optional */

/* start sending the html page */
say "<HTML>"
say "<HEAD>"
say "<TITLE>Sample HTML Page From Rexx</TITLE>"
say "</HEAD>"
say "<BODY>"
say "<H1>Serialization Test for Mod_Rexx</H1>"

say '<p>The Mod_Rexx version string is "'WWWGetVersion()'"'

say "<p>The value of arg(1) is" arg(1)

say '<p>The current RFC 822 formatted time at the server is "'WWWHTTP_time(r)'"'

say '<p>Sleeping 10 seconds'
call SysSleep 10

say '<p>The current RFC 822 formatted time at the server is "'WWWHTTP_time(r)'"'

say "</BODY>"
say "</HTML>"
return OK

