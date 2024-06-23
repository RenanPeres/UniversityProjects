/* <file> - Header File (tyos.c)
 
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



#ifndef TOS_H
#define TOS_H

#define STAGE2_ADDR 0x7e00

#define VIDEO_MEMORY 0xb8000
#define VIDEO_ATTRIBUTE 0X02

/* Core functions.
*  Print          -> Print string str on standard output.
*  Clear          -> Clear the screen.
*  Load 2° Stage  -> Load stage 2 as a block.
*/

/* Print string str on standard output. */

void __attribute((naked, fastcall)) print (const char *str);

/* Clear the screen. */

void __attribute__((naked, fastcall)) clear (void);

/* Load stage 2 as a block. */

void load_stage2_block();

/* Kernel functions.
*  Printnl  -> Print string str on standard output, with a newline.
*  Read     -> Read standard input into buffer.
*  Help     -> Prints a help message.
*  Compare  -> Compare to strings up to BUFFER_MAX_LENGTH-1.
*  Time     -> Read and print system date and time data.
*  Color    -> Change prompt color.
*  Give UP  -> Print ASCII ART.
*  Halt     -> Logical END.
*/

/* Print string str on standard output, with a newline. */

extern char nl[];

#define printnl(str) do{print(str); print (nl);}while(0)

/* Read standard input into buffer. */

void __attribute__((naked, fastcall)) read (char *buffer);

/* Prints a help message. */

void __attribute__((naked)) help (void);

/* Compare to strings up to BUFFER_MAX_LENGTH-1. */

#define BUFFER_MAX_LENGTH 5

int __attribute__((fastcall, naked)) compare (char *s1, char *s2);

/* Time  -> Read and print system date and time data. */

int __attribute__((fastcall, naked)) time (void);

/* Color -> Change prompt color backgroung and font. */

void __attribute__((naked, fastcall)) color (char atual);

/* Give UP  -> Print ASCII ART. */

void giveup();

/* Halt     -> Logical END. */

void halt();

void imprime();

/* Define functions' call. 
*  TIME_CMD 
*  HELP_CMD
*  QUIT_CMD
*  GIVEUP_CMD
*  COLOR_CMD
*  PROMPT
*  NOT_FOUND
*/

#define  TIME_CMD    "time"

#define HELP_CMD     "help"

#define QUIT_CMD     "quit"

#define  GIVEUP_CMD  "giveup"

#define  COLOR_CMD   "color"

#define PROMPT       ">"

#define NOT_FOUND    " not found"


#endif
