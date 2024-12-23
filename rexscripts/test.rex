/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test.rex                                                      */
/*                                                                            */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Test page for the Mod_Rexx Apache module.                     */
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


/* version of this script */
script_version = 'v2.2.0'

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
say "<H1>Sample HTML Page From Rexx</H1>"

say '<p>The Version of this script is' script_version'.'

say '<p>The Mod_Rexx version string is "'WWWGetVersion()'"'

say "<p>The number of Rexx procedure arguments is" arg()

say "<p>The value of arg(1) is" arg(1)

say '<p>The current RFC 822 formatted time at the server is "'WWWHTTP_time(r)'"'

say "<p>The following is some standard Rexx environment information:"
say "<p>The Rexx Version is <b>"v"</b>."
say "<br>The Rexx procedure path/filename is <b>"path"</b>."
say "<br>The Rexx runtime environment name is <b>"env"</b>."
say "<br>The Rexx standard environment name that should be used is <b>"envname"</b>."
say "<br>The starting Address environment is <b>"address()"</b>."
say "<br>The current directory is <b>"directory()"</b>."
say "<br>The current PATH is <br /><b>"value('PATH',,envname)"</b>."

say "<p>The following is the list of standard Rexx CGI variables and their values:"
say '<table border="1"><tr><th>Name</th><th>Value</th></tr>'
say "<tr><td>WWWAUTH_TYPE</td><td>"vorb(wwwauth_type)"</td></tr>"
say "<tr><td>WWWCONTENT_LENGTH</td><td>"vorb(wwwcontent_length)"</td></tr>"
say "<tr><td>WWWCONTENT_TYPE</td><td>"vorb(wwwcontent_type)"</td></tr>"
say "<tr><td>WWWGATEWAY_INTERFACE</td><td>"vorb(wwwgateway_interface)"</td></tr>"
say "<tr><td>WWWHTTP_USER_ACCEPT</td><td>"vorb(wwwhttp_user_accept)"</td></tr>"
say "<tr><td>WWWHTTP_USER_AGENT</td><td>"vorb(wwwhttp_user_agent)"</td></tr>"
say "<tr><td>WWWPATH_INFO</td><td>"vorb(wwwpath_info)"</td></tr>"
say "<tr><td>WWWPATH_TRANSLATED</td><td>"vorb(wwwpath_translated)"</td></tr>"
say "<tr><td>WWWQUERY_STRING</td><td>"vorb(wwwquery_string)"</td></tr>"
say "<tr><td>WWWREMOTE_ADDR</td><td>"vorb(wwwremote_addr)"</td></tr>"
say "<tr><td>WWWREMOTE_HOST</td><td>"vorb(wwwremote_host)"</td></tr>"
say "<tr><td>WWWREMOTE_IDENT</td><td>"vorb(wwwremote_ident)"</td></tr>"
say "<tr><td>WWWREMOTE_USER</td><td>"vorb(wwwremote_user)"</td></tr>"
say "<tr><td>WWWREQUEST_METHOD</td><td>"vorb(wwwrequest_method)"</td></tr>"
say "<tr><td>WWWSCRIPT_NAME</td><td>"vorb(wwwscript_name)"</td></tr>"
say "<tr><td>WWWSERVER_NAME</td><td>"vorb(wwwserver_name)"</td></tr>"
say "<tr><td>WWWSERVER_PORT</td><td>"vorb(wwwserver_port)"</td></tr>"
say "<tr><td>WWWSERVER_PROTOCOL</td><td>"vorb(wwwserver_protocol)"</td></tr>"
say "<tr><td>WWWSERVER_SOFTWARE</td><td>"vorb(wwwserver_software)"</td></tr>"
say "</table>"

say "<p>The following are some additional variables provided to the Rexx program:"
say '<table border="1"><tr><th>Name</th><th>Value</th></tr>'
say "<tr><td>WWWDEFAULT_TYPE</td><td>"vorb(wwwdefault_type)"</td></tr>"
say "<tr><td>WWWFILENAME</td><td>"vorb(wwwfilename)"</td></tr>"
say "<tr><td>WWWFNAMETEMPLATE</td><td>"vorb(wwwfnametemplate)"</td></tr>"
say "<tr><td>WWWIS_MAIN_REQUEST</td><td>"vorb(wwwis_main_request)"</td></tr>"
say "<tr><td>WWWRSPCOMPILER</td><td>"vorb(wwwrspcompiler)"</td></tr>"
say "<tr><td>WWWSERVER_ROOT</td><td>"vorb(wwwserver_root)"</td></tr>"
say "<tr><td>WWWUNPARSEDURI</td><td>"vorb(wwwunparseduri)"</td></tr>"
say "<tr><td>WWWURI</td><td>"vorb(wwwuri)"</td></tr>"
say "</table>"

say "<p>The following are variables which may have been set by the RexxSetVar"
say "directive in the Apache httpd.conf configuration file."
say '<table border="1"><tr><th>Name</th><th>Value</th></tr>'
say "<tr><td>ExampleVar</td><td>"examplevar"</td></tr>"
say "<tr><td>ExampleVar1</td><td>"examplevar1"</td></tr>"
say "<tr><td>ExampleVar2</td><td>"examplevar2"</td></tr>"
say "</table>"

call WWWGetArgs r
say "<p>If the request method is GET then the following array will be filled"
say "with the Query String name/value pairs. If the request method is POST"
say "then the following array will be filled with the POST list of"
say "name/value pairs. The number of arguments is contained in the variable"
say "WWWARGS.0."
say "<p>The number of arguments is" wwwargs.0"."
if wwwargs.0 > 0 then do
   say '<table border="1">'
   do i = 1 to wwwargs.0
      say "<tr><td>WWWARGS."i".!NAME</td><td>"wwwargs.i.!name"</td></tr>"
      say "<tr><td>WWWARGS."i".!VALUE</td><td>"wwwargs.i.!value"</td></tr>"
   end
   say "</table>"
end

call WWWGetCookies r
say "<p>The following array contains all the cookies passed to this request."
say "The number of cookies is contained in the variable"
say "WWWCOOKIES.0."
say "<p>The number of cookies is" wwwcookies.0"."
if wwwcookies.0 > 0 then do
   say '<table border="1">'
   do i = 1 to wwwcookies.0
      say "<tr><td>WWWCOOKIES."i".!NAME</td><td>"wwwcookies.i.!name"</td></tr>"
      say "<tr><td>WWWCOOKIES."i".!VALUE</td><td>"wwwcookies.i.!value"</td></tr>"
   end
   say "</table>"
end

say "<p><hr><p>"

say "The following will reinvoke this script using the POST request method."


say '<FORM method="post" action="./test.rex/extrainfo">'
say '<INPUT type="hidden" name="hiddenfield" value="A hidden string">'
say '<P>Enter some text:'
say '<INPUT type="text" name="visibletext" size="50">'
say '<BR>'
say '<INPUT type="submit">'
say '</FORM>'

say "</BODY>"
say "</HTML>"
return OK


/* vorb: return the value or a required space */
vorb:

if length(arg(1)) > 0 then return arg(1)
else return '&nbsp;'

