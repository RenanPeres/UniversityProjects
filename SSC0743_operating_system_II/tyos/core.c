/* core.c - Included Core Functions stored on MBR
 
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



#include <tyos.h>

/* A string consisting of the CRLF sequence.
   Used by the function-like macro printnl. */

char nl[] = {'\r', '\n', 0x0};

/* Prints string in buffer.  */

void __attribute__((fastcall, naked))  print (const char* buffer)
{
__asm__ volatile
(                                                                          
 "        mov   $0x0e, %%ah           ;" /* Video BIOS service: teletype mode */
 "        mov   $0x0, %%si            ;" 
 "loop%=:                             ;"                                    
 "        mov   (%%bx, %%si), %%al    ;"
 "        cmp   $0x0, %%al            ;" /* Repeat until end of string (0x0). */
 "        je    end%=                 ;"                                    
 "        int   $0x10                 ;" /* Call video BIOS service.          */
 "        add   $0x1, %%si            ;"                                    
 "        jmp   loop%=                ;"                                    
 "end%=:                              ;"
 "        ret                         ;" /* Return from this function.         */

:                        
: [str] "b" (buffer)      /* Var. buffer put in bx, referenced as str .*/
: "ax", "cx", "si"        /* Additional clobbered registers         .  */
 );
}


void __attribute__((naked)) fatal (void)
{
  printnl (nl);
  printnl ("*** Fatal error.");
  __asm__ volatile
    (
     "halt%=:       ;"
     "   hlt;       ;"
     "   jmp halt%= "
     :::
     );
}


/* Clear the screen and set video colors. */

void __attribute__((naked, fastcall)) clear (void)
{

  __asm__ volatile
    (
     " mov $0x0600, %%ax                 ;" /* Video BIOS service: Scroll up. */
     " mov $0x07, %%bh                   ;" /* Attribute (back/foreground.    */
     " mov $0x0, %%cx                    ;" /* Upper-left corner.             */
     " mov $0x184f, %%dx                 ;" /* Upper-right corner.            */
     " int $0x10                         ;" /* Call video BIOS service.       */
     " ret                                " /* Return from function. */
     ::: "ax", "bx", "cx", "dx"		    /* Additional clobbered registers.*/
     );

}


/* Load stage directly after MBR. */

void __attribute__((naked)) load_stage2_block()
{
  
  
  __asm__  volatile
    (

     /* Reset floppy just for the case. */
     
     "reset%=:                    ;"
     "    mov $0x0, %%ah          ;" /* Means reset disk. */
     "    mov $0x0, %%dl          ;" /* Means the floppy disk. */
     "    int $0x13               ;" /* Call BIOS. */
     
     "    jnc load%=             ;" /* On error (CF=1), abort */
     "    call fatal              ;" 

     /* Load stage 2.   */

     " load%=:                    ;"

     "   mov $0x0, %%cx           ;" /* Just to be sure we're in */
     "   mov %%cx, %%ds           ;" /* segment zero. */
     
     "   mov $0x2, %%ah           ;" /* Means read sector. */
     "   mov %[size], %%al        ;" /* Number of sectors to read */
     "   mov $0x0, %%dl           ;" /* Drive    (floppy is 0)*/
     "   mov $0x0, %%ch           ;" /* Cylinder (starts at 0) */
     "   mov $0x0, %%dh           ;" /* Head     (starts at 0) */
     "   mov $0x2, %%cl           ;" /* Sector   (starts at 1) */

     "   mov $0x7e00, %%bx        ;" /* Where to load data. */

     "   int $0x13                ;" /* Call BIOS. */
     "   jc fatal                 ;" /* On error, abort. */

     "   ret                      ;"
     :
     : [size] "n" (STAGE2_SIZE)	/* Defined in Makefile. */
     : "ax"
     );
}

