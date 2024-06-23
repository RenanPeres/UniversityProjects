/* tyos.c - Kernel functions stored on alocated memory
 
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

void halt()
{

  printnl ("Bye!");
  __asm__ volatile
    (
     "loop%=:           ;"
     "        hlt       ;"
     "        jmp loop%=;"
     :::
     );
}

/* MBR implemented functions

/* Read string from terminal into buffer. 

   Note: this function does not check for buffer overrun.
         Buffer size is BUFFER_MAX_LENGTH.

	 Good opportunity for contributing.

*/

void __attribute__((fastcall, naked)) read (char *buffer)
{
  
    __asm__ volatile
    (     
     
     "   mov $0x0, %%si               ;" /* Iteration index for (%bx, %si).  */
     "loop%=:                         ;"
     "   movw $0X0, %%ax              ;" /* Choose blocking read operation.  */
     "   int $0x16                    ;" /* Call BIOS keyboard read service. */
     "   movb %%al, %%es:(%%bx, %%si) ;" /* Fill in buffer pointed by %bx.   */
     "   inc %%si                     ;"
     "   cmp $0xd, %%al               ;" /* Reiterate if not ascii 13 (CR)   */

     "   mov   $0x0e, %%ah            ;" /* Echo character on the terminal.  */
     "   int $0x10                    ;"
     
     "   jne loop%=                   ;"

     " mov $0x0e, %%ah                ;" /* Echo a newline.                  */
     " mov $0x0a, %%al                ;"
     "   int $0x10                    ;"
     
     "   movb $0x0, -1(%%bx, %%si)    ;" /* Add buffer a string delimiter.   */
     "   ret                           " /* Return from function             */
     
     :
     : "b" (buffer) 	          /* Ask gcc to store buffer in %bx          */
     : "ax",  "cx", "si" 	  /* Aditional clobbered registers.          */
     );

  
}

/*  Output a (almost) help message. */

void __attribute__((naked)) help (void)
{
  printnl (" ~~~~~~~~~~~~~~~~~~ Help Center: ~~~~~~~~~~~~~~~~~~ ");
  print(nl);
  printnl ("WHat time is it?                              -> try: 'time'");
  printnl ("Change my OS skin!                            -> try: 'color'");
  printnl ("I will GIVE UP! I DON'T LIKE THIS OS!         -> try: 'giveup'");
  printnl ("So tired, I need to sleep!                    -> try: 'quit'");
   print(nl);
  printnl ("What should I try? (I bet you know this one)  -> try: 'help'");

  __asm__ ("ret");   	   // Naked functions lack return.

}

/* Read system BIOS date and time and print formated buffer on screen */

int __attribute__((fastcall, naked)) time (void)
{
  __asm__ volatile
    (
/* Started read date system */
      " mov $0x04,  %%ah  ;"      /* Selected 'Read System' -> Date */
      " int $0x1A         ;"      /* RTC services interrupt */
      " mov	$0x0e,  %%ah  ;"      /* Video BIOS service */

      " mov %%dl,   %%al  ;"      /* DL -> Contains Day info */
      " shr $0x04,  %%al  ;"      /* Right Shift for decode MSD */
      " add $0x30,  %%al  ;"      /* Convert int value to ASCII symbol */
      
      " int $0x10          ;"     /* Int for print MSD on screen */

      " mov	%%dl,   %%al  ;"      /* Recover day data */
      "	and	$0x0F,  %%al  ;"      /* AND 0x0F for decode LSD */ 
      "	add	$0x30,  %%al  ;"      /* Convert int value to ASCII symbol */

      " int $0x10         ;"      /* Int for print LSD on screen */
      " mov	$0x2F,  %%al  ;"      /* Load '/' on AL */
      " int $0x10         ;"      /* Int for print symbol on screen */

      " mov %%dh,   %%al  ;"      /* DH -> Contains Mounth info */
      " shr $0x04,  %%al  ;"      /* Right Shift for decode MSD */
      " add $0x30,  %%al  ;"      /* Convert int value to ASCII symbol */
      
      " int $0x10          ;"     /* Int for print MSD on screen */

      " mov	%%dh,   %%al  ;"      /* Recover mounth data */
      "	and	$0x0F,  %%al  ;"      /* AND 0x0F for decode LSD */ 
      "	add	$0x30,  %%al  ;"      /* Convert int value to ASCII symbol */

      " int $0x10         ;"      /* Int for print LSD on screen */
      " mov	$0x2F,  %%al  ;"      /* Load '/' on AL */
      " int $0x10         ;"      /* Int for print symbol on screen */

      " mov %%cl,   %%al  ;"      /* CL -> Contains Year info */
      " shr $0x04,  %%al  ;"      /* Right Shift for decode MSD */
      " add $0x30,  %%al  ;"      /* Convert int value to ASCII symbol */
      
      " int $0x10          ;"     /* Int for print MSD on screen */

      " mov	%%cl,   %%al  ;"      /* Recover year data */
      "	and	$0x0F,  %%al  ;"      /* AND 0x0F for decode LSD */ 
      "	add	$0x30,  %%al  ;"      /* Convert int value to ASCII symbol */
/* Ended read date system */
      " int $0x10         ;"      /* Int for print LSD on screen */
      " mov	$0x20,  %%al  ;"      /* Load '<space>' on AL */
      " int $0x10         ;"      /* Int for print symbol on screen */
/* Started read time system */
      " mov $0x02,  %%ah  ;"      /* Selected 'Read System' -> Time */
      " int $0x1A         ;"      /* RTC services interrupt */
      " mov	$0x0e,  %%ah  ;"      /* Video BIOS service */ 

      " mov %%ch,   %%al  ;"      /* CH -> Contains Hour info */
      " shr $0x04,  %%al  ;"      /* Right Shift for decode MSD */
      " add $0x30,  %%al  ;"      /* Convert int value to ASCII symbol */
      
      " int $0x10          ;"     /* Int for print MSD on screen */

      " mov	%%ch,   %%al  ;"      /* Recover hour data */
      "	and	$0x0F,  %%al  ;"      /* AND 0x0F for decode LSD */ 
      "	add	$0x30,  %%al  ;"      /* Convert int value to ASCII symbol */

      " int $0x10         ;"      /* Int for print LSD on screen */
      " mov	$0x3A,  %%al  ;"      /* Load ':' on AL */
      " int $0x10         ;"      /* Int for print symbol on screen */

      " mov %%cl,   %%al  ;"      /* CL -> Contains Minute info */
      " shr $0x04,  %%al  ;"      /* Right Shift for decode MSD */
      " add $0x30,  %%al  ;"      /* Convert int value to ASCII symbol */
      
      " int $0x10          ;"     /* Int for print MSD on screen */

      " mov	%%cl,   %%al  ;"      /* Recover minute data */
      "	and	$0x0F,  %%al  ;"      /* AND 0x0F for decode LSD */ 
      "	add	$0x30,  %%al  ;"      /* Convert int value to ASCII symbol */

      " int $0x10          ;"     /* Int for print LSD on screen */
/* Ended read time system */
      " ret               ;"
      ::: "ax", "cx", "dx" 	      /* Aditional clobbered registers. */
     );
}

/* Compare two strings up to position BUFFER_MAX_LENGTH-1. */

int __attribute__((fastcall, naked)) compare (char *s1, char *s2)
{
  __asm__ volatile
    (
      "    mov %[len], %%cx   ;"
      "    mov $0x1, %%ax     ;"
      "    cld                ;"
      "    repe  cmpsb        ;"
      "    jecxz  equal       ;"             
      "    mov $0x0, %%ax     ;"
      "equal:                 ;"
      "    ret                ;"
      :
      : [len] "n" (BUFFER_MAX_LENGTH-1), /* [len] is a constant.   */
	"S" (s1),		/* Ask gcc to store s1 in %si      */
	"D" (s2)		/* Ask gcc to store s2 is %di      */
      : "ax", "cx", "dx"	/* Additional clobbered registers. */
     );

  return 0;                /* Bogus return to fulfill funtion's prototype.*/
}


/* Change color backgrounf and font
*   Note1: this function does not check for pass option, then there is only one change cicle.
*   Note2: this function doenst turn entire prompt colored, just a frame of it.
* 
*/

void __attribute__((fastcall, naked)) color (char atual)
{
  if(atual == '1'){                         /* Aqua and Yellow            */
    __asm__ volatile
      (
      " mov $0x06,    %%ah                ;" /* Scroll up.                */
      " xor %%al,     %%al                ;" /* Clear entire screen.      */
      " xor %%cx,     %%cx                ;" /* Upper-left corner.        */
      " mov $0x184F,  %%dx                ;" /* Upper-right corner.       */
      " mov $0x36,    %%bh                ;" /* Attribute (aqua/yellow).  */
      " int $0x10                         ;" /* Call video BIOS service.  */
      " ret                                " /* Return from function.     */
      ::: "ax", "bx", "cx", "dx"		    /* Additional clobbered registers.*/
      );
  }else if(atual == '2'){                    /* Red and Green             */
    __asm__ volatile
      (
      " mov $0x06,    %%ah                ;" /* Scroll up.                */
      " xor %%al,     %%al                ;" /* Clear entire screen.      */
      " xor %%cx,     %%cx                ;" /* Upper-left corner.        */
      " mov $0x184F,  %%dx                ;" /* Upper-right corner.       */
      " mov $0x42,    %%bh                ;" /* Attribute (red/green).    */
      " int $0x10                         ;" /* Call video BIOS service.  */
      " ret                                " /* Return from function.     */
      ::: "ax", "bx", "cx", "dx"		    /* Additional clobbered registers.*/
      );

  }else if(atual == '3'){                    /* Grey and Black            */
    __asm__ volatile
      (
      " mov $0x06,    %%ah                ;" /* Scroll up.                */
      " xor %%al,     %%al                ;" /* Clear entire screen.      */
      " xor %%cx,     %%cx                ;" /* Upper-left corner.        */
      " mov $0x184F,  %%dx                ;" /* Upper-right corner.       */
      " mov $0x70,    %%bh                ;" /* Attribute (grey/black).   */
      " int $0x10                         ;" /* Call video BIOS service.  */
      " ret                                " /* Return from function.     */
      ::: "ax", "bx", "cx", "dx"		    /* Additional clobbered registers.*/
      );

  }else{                                     /* Default                   */
    __asm__ volatile
      (
      " mov $0x06,    %%ah                ;" /* Scroll up.                */
      " xor %%al,     %%al                ;" /* Clear entire screen.      */
      " xor %%cx,     %%cx                ;" /* Upper-left corner.        */
      " mov $0x184F,  %%dx                ;" /* Upper-right corner.       */
      " mov $0x07,    %%bh                ;" /* Attribute (black/white).  */
      " int $0x10                         ;" /* Call video BIOS service.  */
      " ret                                " /* Return from function.     */
      ::: "ax", "bx", "cx", "dx"		    /* Additional clobbered registers.*/
      );
  }return;
}

void giveup(){
  printnl("Never_Gonna_Give_You_Up.mp4      ........");
  printnl("                           ...................");
  printnl("                       ........................");
  printnl("                     ............................");
  printnl("                    ..............................");
  printnl("                   ..........                .......");
  printnl("                   ......                     .....");
  printnl("                   ......                      ....");
  printnl("                   ......     ......    ......  ...");
  printnl("                   ......    .......     ....   ...");
  printnl("                    .....                       ..");
  printnl("                    .. ..        ....            ..");
  printnl("                     .                          .. ");
  printnl("                                  ........     . .");
  printnl("                       ....             .");
  printnl("                         .          ...      .");
  printnl("                         ..   .            . ");
  printnl("                       ....................  ..");
  printnl("                   ..................      ....");
  printnl("            ........................   .    .........");
  printnl("         ..........................       ................");
  printnl("       .........................    ..    ...................");
  printnl("      .........................             .................. ");
  printnl("      .......................            .....................");
  return;
}