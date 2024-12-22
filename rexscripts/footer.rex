/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module:      footer.rex                                                    */
/*                                                                            */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Adds a footer to a standard HTML page via the Mod_Rexx Apache */
/*              Module.                                                       */
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
OK        = 0      /* Module has handled this stage. */
NOT_FOUND = 404    /* Main document not found. */

/* get the Apache request record ptr */
r = arg(1)

body = '</BODY>'

/* try to open the main document HTML file */
retc = stream(wwwfilename, 'c', 'open read')
if retc <> 'READY:' then do
   return NOT_FOUND
   end

/* set content-type and send the HTTP header */
call WWWSendHTTPHeader r, "text/html"
call WWWGetArgs r

/* read in the document and look for the </BODY> tag */
call on notready name seteof
eof = 0
line = linein(wwwfilename)
x = pos(body, translate(line))
do while x = 0 & eof = 0
   say line
   line = linein(wwwfilename)
   x = pos(body, translate(line))
   end
if eof = 1 then return OK

/* we found a </BODY> tag so send out the footer */
say substr(line, 1, x - 1)
call send_footer
say substr(line, x)

/* read in the rest of the document */
line = linein(wwwfilename)
do while eof = 0
   say line
   line = linein(wwwfilename)
   end

/* we are done */
return OK


/* function which indicates end-of-file */
seteof:
eof = 1
return


/* send the footer */
send_footer:
say '<br />'
say '<hr />'
say '<br />'
say '&copy; 2001 <a href="http://www.ibm.com/">IBM Corporation</a>'
say '<br />'
say '<em>Last Modified:' date()'</em>'
say '<br />'
say '<em>Footer added by Mod_Rexx.</em>'
say '<br />'
return

