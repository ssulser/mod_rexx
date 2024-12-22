/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module:      CodeRed.rex                                                   */
/*                                                                            */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Handle a CodeRed request via the Mod_Rexx Apache module.      */
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
/* This program handles a request by the Code Red worm and notifies           */
/* SecurityFocus that a Code Red request has been generated from that         */
/* client. This handler requires the following in the httpd.conf file:        */
/*                                                                            */
/*    <Location /default.ida>                                                 */
/*       SetHandler rexx_handler                                              */
/*       RexxHandler '/opt/IBMHTTPDServer/cgi-bin/CodeRed.rex'                */
/*    </Location>                                                             */
/*                                                                            */
/* Things you will need to modify:                                            */
/*                                                                            */
/* 1. You need a copy of SMTPMail.cmd and you will need to change the         */
/*    location path in this pgm to point to its location on your server.      */
/* 2. You need to change the value assigned to the variable smtp_server to    */
/*    point to your SMTP Server.                                              */
/* 3. You need to change the sender and possibly the receiver information on  */
/*    the e-mail message.                                                     */
/* 4. You need to change the location for the temporary files (fname in the   */
/*    main pgm and fname in the in_cache subroutine).                         */
/*                                                                            */
/* All the needed modification statements have been tagged with the string    */
/* '**change**' in a Rexx comment prior to the statement.                     */
/*                                                                            */
/* The idea for this example Rexx handler comes from Reuven M. Lerner in a    */
/* document located at                                                        */
/* http://www.oreillynet.com/pub/a/apache/2001/08/16/code_red.html.           */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* These are all the variables that need to be */
/* modified to support your system environment */
/***change***/ sender = 'Your Name <yourname@youraddress.com>'
/***change***/ smtp_server = '127.0.0.1'
/***change***/ fname = '/var/tmp/' || date('B') || time('S') || '.txt'
/***change***/ fname2 = '/var/tmp/CodeRedCache.txt'

/* Version of this script */
script_version = 'v2.2.0'

/* Load rxsock if necessary */
if rxfuncquery('SockDropFuncs') then do
   call RxFuncAdd 'SockLoadFuncs', 'rxsock', 'SockLoadFuncs'
   call SockLoadFuncs
   call SockInit
   end

/* These are some typical Apache return codes */
OK        = 0      /* Module has handled this stage. */
FORBIDDEN = 403    /* Main document forbidden. */
NOT_FOUND = 404    /* Main document not found. */

/* Get the Apache request record ptr */
r = arg(1)

/* filter out some ip addresses */
if wwwremote_addr = '' then return FORBIDDEN /* this can happen during testing */
if filter_ip(wwwremote_addr) = 1 then return FORBIDDEN
if in_cache(wwwremote_addr) = 1 then return FORBIDDEN

/* wwwremote_host will be blank if Apache was not able to look up the name */
if wwwremote_host = '' then do
   /* Look up the host name */
   call SockGetHostByAddr wwwremote_addr, 'host.!'
   if symbol('host.!name') = 'LIT' then wwwremotehost = 'UnknownHostName'
   else wwwremote_host = host.!name
   end

/* set up some stuff for the e-mail */

/* Now send an e-mail message to SecurityFocus */
receiver = 'aris-report@securityfocus.com'
subject = 'CodeRed infection on' wwwremote_host': Automatic report'

/* Send the e-mail message to a temp file first */
call stream fname, 'c', 'open write replace'
call lineout fname, 'From:' sender
call lineout fname, 'To:' receiver
call lineout fname, 'Subject:' subject
call lineout fname, wwwremote_addr || d2c(9) || WWWHTTP_time(r)
call lineout fname, ''
call lineout fname, 'Automatically generated by CodeRed.rex for' wwwgateway_interface 'and'
call lineout fname, 'Apache, written by W. David Ashley (<dashley@us.ibm.com> and running'
call lineout fname, 'on' wwwserver_software'.'
call lineout fname, ''
call stream fname, 'c', 'close'

/* now send the email message */
/***change***/ call '/pub/Apps/RexxMisc/smtpmail.cmd' fname smtp_server
if rc <> 0 then do
   call WWWLogError r, 'SMTPMail.cmd error sending file' fname'.'
   end
else do
   call WWWLogInfo r, 'Successfully mailed file' fname'.'
   end

/* we are done */
return FORBIDDEN


/*----------------------------------------------------------------------------*/
/* Filter_ip                                                                  */
/*----------------------------------------------------------------------------*/

filter_ip:
ip = arg(1)
select
   when substr(ip, 1, 3) = '10.' then return 1
   when substr(ip, 1, 8) = '192.168.' then return 1
   otherwise nop
   end
return 0


/*----------------------------------------------------------------------------*/
/* In_cache                                                                   */
/*----------------------------------------------------------------------------*/

in_cache: procedure expose fname2

/* set up some defaults */
ip = arg(1)
cache.0 = 0

/* does cache file exist? */
path = stream(fname2, 'c', 'query exists')
if path <> '' then do
   /* read in the cache file */
   eof = 0
   call on notready name seteof
   ipaddr = linein(fname2)
   do while eof = 0
      i = cache.0 + 1
      cache.i = ipaddr
      cache.0 = i
      ipaddr = linein(fname2)
      end
   call stream fname2, 'c', 'close'
   end

/* does the entry exist in the cache file? */
do i = 1 to cache.0
   if ip = cache.i then return 1 /* entry exists */
   end

/* entry does not exist so append it to the end of the file and return */
call stream fname2, 'c', 'open write append'
call lineout fname2, ip
call stream fname2, 'c', 'close'
return 0


/*----------------------------------------------------------------------------*/
/* Seteof                                                                     */
/*----------------------------------------------------------------------------*/

seteof:
eof = 1
return

