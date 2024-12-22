/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module:      Apache.cls                                                    */
/*                                                                            */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: ooRexx class to support Mod_Rexx Apache request, server       */
/*              and connection objects. It uses a set of Rexx external        */
/*              functions to gain access to the Apache structures. It         */
/*              enforces certain ruls concerning assigning values to the      */
/*              structures.                                                   */
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


/*----------------------------------------------------------------------------*/
/* The Request_rec class                                                      */
/*----------------------------------------------------------------------------*/

::class Request_rec public

::method init
/* initialize the instance */
expose r
use arg r
return

::method connection
/* return the r -> connection value */
expose r
return WWWReqRecConnection(r)

::method server
/* return the r -> server value */
expose r
return WWWReqRecServer(r)

::method next
/* return the r -> next value */
expose r
return WWWReqRecNext(r)

::method prev
/* return the r -> prev value */
expose r
return WWWReqRecPrev(r)

::method main
/* return the r -> main value */
expose r
return WWWReqRecMain(r)

::method ismain
/* returns true if this is the main request */
/* Because of a limitation in the Mod_Rexx package, */
/* this method will always return .true             */
expose r
return (WWWReqRecIsMain(r) <> 0)

::method the_request
/* return the r -> the_request value */
expose r
return WWWReqRecThe_request(r)

::method proxyreq
/* return the r -> proxyreq value */
expose r
return (WWWReqRecProxyreq(r) <> 0)

::method header_only
/* return the r -> header_only value */
expose r
return (WWWReqRecHeader_only(r) <> 0)

::method protocol
/* return the r -> protocol value */
expose r
return WWWReqRecProtocol(r)

::method status_line
/* return/set the r -> status_line value */
expose r
if arg() > 0 then do
   use arg new_statusline
   call WWWReqRecStatus_line r, new_statusline
   return new_statusline
   end
return WWWReqRecStatus_line(r)

::method status
/* return/set the r -> status value */
expose r
if arg() > 0 then do
   use arg new_status
   call WWWReqRecStatus r, new_status
   return new_status
   end
return WWWReqRecStatus(r)

::method method
/* return/set the r -> method value */
expose r
if arg() > 0 then do
   use arg new_method
   call WWWReqRecMethod r, new_method
   return new_method
   end
return WWWReqRecMethod(r)

::method method_number
/* return/set the r -> method_number value */
expose r
if arg() > 0 then do
   use arg new_method_number
   call WWWReqRecMethod r, new_method_number
   return new_method_number
   end
return WWWReqRecMethod_number(r)

::method allowed
/* return/set the r -> allowed value */
expose r
if arg() > 0 then do
   use arg new_allowed
   call WWWReqRecAllowed r, new_allowed
   return new_allowed
   end
return WWWReqRecAllowed(r)

::method bytes_sent
/* return the r -> bytes_sent value */
expose r
return WWWReqRecBytes_sent(r)

::method header_in
/* return/set the r -> headers_in value */
expose r
if arg() = 1 then do
   use arg key
   return WWWReqRecHeader_in(r, key)
   end
if arg() = 2 then do
   use arg key, new_value
   return WWWReqRecHeader_in(r, key, new_value)
   end
return ''

::method header_out
/* return/set the r -> headers_out value */
expose r
if arg() = 1 then do
   use arg key
   return WWWReqRecHeader_out(r, key)
   end
if arg() = 2 then do
   use arg key, new_value
   return WWWReqRecHeader_out(r, key, new_value)
   end
return ''

::method err_header_out
/* return the r -> err_headers_out value */
expose r
if arg() = 1 then do
   use arg key
   return WWWReqRecErr_header_out(r, key)
   end
if arg() = 2 then do
   use arg key, new_value
   return WWWReqRecErr_header_out(r, key, new_value)
   end
return ''

::method subprocess_env
/* return/set the r -> subprocess_env value */
expose r
if arg() = 1 then do
   use arg key
   return WWWReqRecSubprocess_env(r, key)
   end
if arg() = 2 then do
   use arg key, new_value
   return WWWReqRecSubprocess_env(r, key, new_value)
   end
return ''

::method notes
/* return the r -> notes value */
expose r
if arg() = 1 then do
   use arg key
   return WWWReqRecNotes(r, key)
   end
if arg() = 2 then do
   use arg key, new_value
   return WWWReqRecNotes(r, key, new_value)
   end
return ''

::method content_type
/* return the r -> content_type value */
expose r
if arg() = 1 then do
   use arg new_content_type
   call WWWReqRecContent_type r, new_content_type
   return new_content_type
   end
return WWWReqRecContent_type(r)

::method content_encoding
/* return the r -> content_encoding value */
expose r
if arg() = 1 then do
   use arg new_content_encoding
   call WWWReqRecContent_encoding r, new_content_encoding
   return new_content_encoding
   end
return WWWReqRecContent_encoding(r)

::method handler
/* return the r -> handler value */
expose r
if arg() = 1 then do
   use arg new_handler
   call WWWReqRecHandler r, new_handler
   return new_handler
   end
return WWWReqRecHandler(r)

::method content_languages
/* return the r -> content_languages value */
expose r
if arg() = 1 then do
   use arg new_content_languages
   call WWWReqRecContent_languages r, new_content_languages
   return new_content_languages
   end
return WWWReqRecContent_languages(r)

::method no_cache
/* return the r -> no_cache value */
expose r
return (WWWReqRecNo_cache(r) <> 0)

::method uri
/* return the r -> uri value */
expose r
if arg() = 1 then do
   use arg new_uri
   call WWWReqRecUri r, new_uri
   return new_uri
   end
return WWWReqRecUri(r)

::method filename
/* return the r -> filename value */
expose r
if arg() = 1 then do
   use arg new_filename
   call WWWReqRecFilename r, new_filename
   return new_filename
   end
return WWWReqRecFilename(r)

::method path_info
/* return the r -> path_info value */
expose r
if arg() = 1 then do
   use arg new_path_info
   call WWWReqRecPath_info r, new_path_info
   return new_path_info
   end
return WWWReqRecPath_info(r)

::method args
/* return the r -> args value */
expose r
return WWWReqRecArgs(r)

::method finfo_stmode
/* return the r -> finfo.st_mode value */
expose r
return WWWReqRecFinfo_stmode(r)

::method user
/* return the r -> user value */
expose r
return WWWReqRecUser(r)

::method auth_type
/* return the r -> ap_auth_type value */
expose r
return WWWReqRecAuth_type(r)


/*----------------------------------------------------------------------------*/
/* The Server_rec class                                                       */
/*----------------------------------------------------------------------------*/

::class Server_rec public

::method init
/* initialize the instance */
expose s
use arg s
return

::method server_admin
/* return the s -> server_admin value */
expose s
return WWWSrvRecServer_admin(s)

::method server_hostname
/* return the s -> server_hostname value */
expose s
return WWWSrvRecServer_hostname(s)

::method port
/* return the s -> port value */
expose s
return WWWSrvRecPort(s)

::method is_virtual
/* return the s -> is_virtual value */
expose s
return (WWWSrvRecIs_virtual(s) <> 0)


/*----------------------------------------------------------------------------*/
/* The Connection_rec class                                                   */
/*----------------------------------------------------------------------------*/

::class Connection_rec public

::method init
/* initialize the instance */
expose c
use arg c
return

::method user
/* return the c -> user value */
expose c
return WWWCnxRecUser(c)

::method auth_type
/* return the c -> ap_auth_type value */
expose c
return WWWCnxRecAuth_type(c)

::method aborted
/* return the c -> aborted value */
expose c
return (WWWCnxRecAborted(c) <> 0)

