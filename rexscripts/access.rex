/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module:      access.rex                                                    */
/*                                                                            */
/* Author:      W. David Ashley                                               */
/*                                                                            */
/* Description: This program sheck that a client ip address is valid to       */
/*              receive the specified page.                                   */
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
/*----------------------------------------------------------------------------*/


/* version of this script */
script_version = 'v2.4.0'

/* these are some typical Apache return codes */
OK        = 0      /* Module has handled this stage. */
FORBIDDEN = 403    /* Main document forbidden. */

/* get the Apache request record ptr */
r = arg(1)

/* we add the ending period so Rexx will see it as a string */
iprange = '192.168.'
if substr(wwwremote_addr, 1, length(iprange)) <> iprange then return FORBIDDEN

/* we are done */
return OK

