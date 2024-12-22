#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module:      make_mod_rexx.rex                                             */
/*                                                                            */
/* Author:      The Programmers' Guild, Inc.                                  */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: Rexx program which will make a Mod_Rexx binary based on your  */
/*              operating system and Rexx interpreter.                        */
/*                                                                            */
/* Notes: This script assumes the the user has write access to the current    */
/*        directory so that files can be created and removed from the current */
/*        directory.                                                          */
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
/*----------------------------------------------------------------------------*/


/* find out the operating system */
parse source source_str
parse var source_str os .
if substr(os, 1, 7) = 'Windows' then os = 'WINDOWS'
if os = 'WIN32' then os = 'WINDOWS' /* for Regina */
if os = 'UNIX' then os = 'LINUX'    /* for Regina */
select
   when os = 'OS/2' then do
      say 'Error: OS/2 is no longer supported by Mod_Rexx.'
      return 8
      end
   when os = 'LINUX' then nop
   when os = 'AIX' then nop
   when os = 'WINDOWS' then nop
   otherwise do
      say 'Error: Unknown operating system.'
      return 12
      end
   end

/* get any command line arguments */
parse arg arguments
parse upper var arguments rexx_interpreter .
if rexx_interpreter = '' then rexx_interpreter = 'HELP'
select
   when rexx_interpreter = 'HELP' then do
      call helpmsg
      return 0
      end
   when rexx_interpreter = 'OREXX' then rexx_interpreter = 'OOREXX'
   when rexx_interpreter = 'OOREXX' then nop
   when rexx_interpreter = 'REGINA' then nop
   when rexx_interpreter = 'REXXTRANS' then nop
   otherwise do
      say 'Error: Invalid Rexx interpreter argument found.'
      return 8
      end
   end
if os = 'AIX' & rexx_interpreter = 'REGINA' then do
   say 'Error: Regina is not supported in the AIX environment.'
   return 8
   end

/* now call the appropriate functions for making Mod_Rexx */
select
   when os = 'LINUX' then do
      'cp ./makefile.linux ./makefile'
      if rexx_interpreter = 'OOREXX' then do
         'make REXXINT='rexx_interpreter
         end
      else if rexx_interpreter = 'REGINA' then do
         'make REXXINT='rexx_interpreter
         end
      else if rexx_interpreter = 'REXXTRANS' then do
         'make REXXINT='rexx_interpreter
         end
      else nop
      'cp mod_rexx.so ./bin'
      'make cleanall'
      'rm ./makefile'
      end
   when os = 'AIX' then do
      'cp ./makefile.aix ./makefile'
      if rexx_interpreter = 'OOREXX' then do
         'make -DREXXINT='rexx_interpreter
         end
      else nop
      'cp mod_rexx.so ./bin'
      'make cleanall'
      'rm ./makefile'
      end
   when os = 'WINDOWS' then do
      if rexx_interpreter = 'OOREXX' then do
         'nmake /F makefile.w10 REXXINT='rexx_interpreter
         end
      else if rexx_interpreter = 'REGINA' then do
         'nmake /F makefile.w10 REXXINT='rexx_interpreter
         end
      else if rexx_interpreter = 'REXXTRANS' then do
         'nmake /F makefile.w10 REXXINT='rexx_interpreter
         end
      else nop
      'copy mod_rexx.dll .\bin'
      'nmake /F makefile.nt cleanall'
      end
   otherwise nop
   end

return 0


helpmsg:
say
if os = 'AIX' then do
   say 'Syntax: make_mod_rexx help|oorexx'
   say
   say 'The help argument will give you this message.'
   say 'The oorexx argument will cause Mod_Rexx to be built & bound to Object Rexx.'
   end
else do
   say 'Syntax: rexx make_mod_rexx.rex help|oorexx|regina|rexxtrans'
   say
   say 'The help argument will give you this message.'
   say 'The oorexx argument will cause Mod_Rexx to be built & bound to Object Rexx.'
   say 'The regina argument will cause Mod_Rexx to be built & bound to Regina Rexx.'
   say 'The rexxtrans argument will cause Mod_Rexx to be built & bound to RexxTrans.'
   end
say
return

