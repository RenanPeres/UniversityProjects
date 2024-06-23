/* mbr.h - Header file (mbr.c)
 
   Copyright (c) 2021, Hiago de Franco Moreira <https://github.com/hiagofranco>
   Copyright (c) 2021, Renan Peres Martins <https://github.com/RenanPeres>

   This piece of software is a derivative work of SYSeg, by Monaco F. J.
   SYSeg is distributed under the license GNU GPL v3, and is available
   at the official repository https://www.gitlab.com/monaco/syseg.

   This file is part of soii21-rh_OS.

   soii21-rh_OS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef MBR_H
#define MBR_H

/* Core functions.
*  Print    -> Print the null-terminated buffer on standard output.
*  Printnl  -> Print the null-terminated string buffer on standard output, and a newline.
*  Clear    -> Clear the screen.
*  Read     -> Read standard input into fixed-length buffer (no check).
*  Compare  -> Compare to strings up to BUFFER_MAX_LENGTH-1.
*  Time     -> Read and print system date and time data.
*/

/* Print    -> Print the null-terminated buffer on standard output. */

void __attribute((naked, fastcall)) print (const char *buffer);

/* Printnl  -> Print the null-terminated string buffer on standard output, and a newline. */

extern char nl[];

#define printnl(str) do{print(str); print (nl);}while(0)

/* Clear    -> Clear the screen. */

void __attribute__((naked, fastcall)) clear (void);

/* Read     -> Read standard input into fixed-length buffer (no check). */

void __attribute__((naked, fastcall)) read (char *buffer);

/* Compare  -> Compare to strings up to BUFFER_MAX_LENGTH-1. */

#define BUFFER_MAX_LENGTH 5

int __attribute__((fastcall, naked)) compare (char *s1, char *s2);

/* Time     -> Read and print system date and time data. */

int __attribute__((fastcall, naked)) time (void);

/* Define functions' call. 
*  TIME_CMD 
*  quit()
*  QUIT_CMD
*  PROMPT
*  NOT_FOUND
*/

#define  TIME_CMD "time"

#define quit() printnl("Bye!")

#define QUIT_CMD "quit"

#define PROMPT ">"

#define NOT_FOUND " not found"

#endif	/* MBR_H */
