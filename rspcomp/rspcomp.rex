/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 * Module:      rspcomp.rex
 *
 * Author:      Terry Fuller
 * Author:      W. David Ashley
 *
 * Description: Compile an *.rsp (Rexx Server Page) into a real Rexx pgm.
 *
 * Copyright (C) The Programmers' Guild, Inc. 2021-2022. All Rights Reserved
 * Copyright (C) W. David Ashley 2004-2007. All Rights Reserved.
 *
 * This software is subject to the terms of the Commom Public License. You
 * must accept the terms of this license to use this software. Refer to
 * the file CPLv1.0.htm included in this package for more information.
 *
 * The program is provided "as is" without any warranty express or implied,
 * including the warranty of non-infringement and the implied warranties of
 * merchantibility and fitness for a particular purpose.
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Global option variables:                                                   */
/*                                                                            */
/* errmsg   = 0  do not show error messages (default)                         */
/*            1  output error messages to stdout                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Global state variable:                                                     */
/*                                                                            */
/* state    = 0  parsing HTML lines (starting state)                          */
/*            1  parsing Rexx statements delimited by '<?rexx' and '?>' tags  */
/*            2  parsing Rexx statements delimited by                         */
/*               '<script type="rexx">' and '</script>' tags                  */
/*                                or                                          */
/*               '<script language="rexx">' and '</script>' tags              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Return codes:                                                              */
/*                                                                            */
/* 0  successful execution                                                    */
/* 1  no output Rexx filename specified                                       */
/* 2  the input RSP file was not found                                        */
/* 3  cannot open the RSP input file                                          */
/* 4  cannot open the Rexx output file                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* version of this script */
script_version = 'v2.4.0'

/* set global variables and option defaults */
errmsg = 0
parse upper version rexx_ver

/* get the input filename arguments and options */
cmdline = arg(1)
if substr(cmdline, 1, 1) = '"'
  then parse var cmdline '"' rspfilename '"' cmdline
  else parse var cmdline rspfilename cmdline
cmdline = strip(cmdline)
if substr(cmdline, 1, 1) = '"'
  then parse var cmdline '"' rexfilename '"' option1 .
  else parse var cmdline rexfilename option1 .
if rexfilename = '' then do
   call console_msg 'Error: No output Rexx filename specified.'
   return 1
   end
if translate(option1) = 'ERRMSG' then errmsg = 1

/* try to open the rsp file */
if rspfilename = '' then do
   call console_msg 'Error: No input RSP File specified.'
   return 2
   end

/* use .stream object to avoid case-insensitive file name performance issue -- taf 2021/02/12 */

  retc = stream(rspfilename, 'c', 'open read')
  if retc <> 'READY:' then
     do
     call console_msg 'Error: cannot open file' rspfilename
     return 3
     end

  retc = stream(rexfilename, 'c', 'open write')
  if retc <> 'READY:' then
    do
    call console_msg 'Error: cannot open file' rexfilename
    return 4
    end

/* read in the rsp file and look for the the rsp tags */
state = 0 /* initial state is to output HTML lines */
call Rexx_pgm_header
/* set up an end-of-file indicator on the input rsp file */
eof = 0
call on notready name seteof
line = iput(rspfilename)
  do while eof = 0
  call process_line line
  line = iput(rspfilename)
  if eof=1 then leave
  end

/* close the files */
  call stream rspfilename, 'c', 'close'
  call stream rexfilename, 'c', 'close'

/* done */
return 0


/*----------------------------------------------------------------------------*/
/* function which indicates end-of-file                                       */
/*----------------------------------------------------------------------------*/

seteof:
eof = 1
return eof


/*----------------------------------------------------------------------------*/
/* process a line from the rsp file                                           */
/*----------------------------------------------------------------------------*/

process_line: procedure expose state rexfilename
line = arg(1)
uline = translate(strip(line))
select
   when state = 0 then do
      if uline = '<?REXX' then do
         /* change state to output Rexx pgm line(s) */
         state = 1
         end
      else if substr(uline, 1, 8) = '<SCRIPT ' then do
         parse var uline '<SCRIPT' . 'TYPE=' lang . '>'
         if lang = '' then ,
          parse var uline '<SCRIPT' . 'LANGUAGE=' lang . '>'
         lang = strip(lang,, '"')
         lang = strip(lang,, "'")
         if lang = 'REXX' then do
            /* change state to output Rexx pgm line(s) */
            state = 2
            end
         else nop /* must be another language (like Javascript) */
         end
      if state = 0 then do
         /* state has not changed, output the HTML line */
         call splitline line
         /* see if we are at the end of the HTML document */
         if pos('</HTML>', uline) > 0 then do
            /* doing this separates the mainline code from any Rexx     */
            /* subroutines/functions added at the end of the RSP file,  */
            /* that way we do not just fall into them (a Rexx error).   */
            call Rexx_pgm_footer
            end

         end
      end
   when state = 1 then do
      if uline = '?>' then do
         /* change state to output HTML line(s) */
         state = 0
         end
      else do
         /* output the Rexx pgm line */
         call oput rexfilename, line
         end
      end
   when state = 2 then do
      if uline = '</SCRIPT>' then do
         /* change state to output HTML line(s) */
         state = 0
         end
      else do
         /* output the Rexx pgm line */
         call oput rexfilename, line
         end
      end
   otherwise nop
   end
return


/*----------------------------------------------------------------------------*/
/* split a single HTML line into multiple lines if necessary                  */
/* (so Rexx maximum program line length is not exceeded)                      */
/*----------------------------------------------------------------------------*/

splitline: procedure expose rexfilename
line = arg(1)
do while length(line) > 90
   x = substr(line, 1, 80)
   do i = 81 to length(line)
      if substr(line, i, 1) <> ' ' then x = x || substr(line, i, 1)
      else leave
      end
   call oput rexfilename, "say '" || add_quotes("'", x) || "'"
   line = substr(line, i)
   end
call oput rexfilename, "say '" || add_quotes("'", line) || "'"
return


/*----------------------------------------------------------------------------*/
/* add quotes to a string (two for one)                                       */
/*----------------------------------------------------------------------------*/

add_quotes: procedure
quote = arg(1)
linex = arg(2)
zx = pos(quote, linex)
do while zx > 0
   linex = substr(linex, 1, zx) || quote || substr(linex, zx + 1)
   zx = pos(quote, linex, zx + 2)
   end
return linex


/*----------------------------------------------------------------------------*/
/* add header information to the Rexx program                                 */
/*----------------------------------------------------------------------------*/

Rexx_pgm_header:
call oput rexfilename, '/* Start of Rexx RSP header information */'
call oput rexfilename, '/*'
call oput rexfilename, ''

call oput rexfilename, 'RSP Compiler Information:'
call oput rexfilename, '   Compiler version: ' script_version
call oput rexfilename, '   Compile timestamp:' date() time()
call oput rexfilename, '   Operating System: ' os
call oput rexfilename, '   Rexx Version:     ' rexx_ver
call oput rexfilename, ''

call oput rexfilename, 'Source RSP File Information:'
fsize = stream(rspfilename, 'c', 'query size')
ftimestamp = stream(rspfilename, 'c', 'query timestamp')
call oput rexfilename, '   ' || ftimestamp || '  ' || format(fsize, 10) || ,
                          '  ' || stream(rspfilename, 'c', 'query exists')
call oput rexfilename, ''

call oput rexfilename, 'Output Rexx Program File Information:'
call oput rexfilename, '   Filename:' stream(rexfilename, 'c', 'query exists')
call oput rexfilename, ''

call oput rexfilename, '*/'

call oput rexfilename, ''
call oput rexfilename, '/* note: the following call is NOT optional */'
call oput rexfilename, 'call WWWSendHTTPHeader arg(1), "text/html"'
call oput rexfilename, '/* note: these calls are optional but here anyway */'
call oput rexfilename, 'call WWWGetArgs arg(1)'
call oput rexfilename, 'call WWWGetCookies arg(1)'
call oput rexfilename, ''
call oput rexfilename, '/* End of Rexx RSP header information */'

call oput rexfilename, ''
return


/*----------------------------------------------------------------------------*/
/* add footer information to the Rexx program                                 */
/*----------------------------------------------------------------------------*/

Rexx_pgm_footer:
call oput rexfilename, '/* Start of Rexx RSP footer information */'
call oput rexfilename, 'return 0'
call oput rexfilename, '/* End of Rexx RSP footer information */'
return


/*----------------------------------------------------------------------------*/
/* send message to the console (stdout)                                       */
/*----------------------------------------------------------------------------*/

console_msg:
if errmsg = 1 then say arg(1)
return

/* routine to cope with stream v .stream output */
oput: procedure
parse arg fn,line
call lineout fn,line
return result

/* routine to cope with stream v .stream input */
iput: procedure expose eof
parse arg fn
return linein(fn)
