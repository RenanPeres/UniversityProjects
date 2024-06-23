/* <file> - <One-line note about this file>
 
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



/*  Linker script. */

OUTPUT_FORMAT(binary)		/* Output flat binary (no structure)          */
SECTIONS
{
        . = 0x7C00;		/* Line counter is now 0x7c00.                */
	
        .text :			/* Ouput file has only one (implied) section. */
	{
	  *(.text)              /* Copy input section .text to the oputput.   */
	  *(.data)		/* Copy input section .data to the output.    */
	  *(.bss)		/* Copy input section .bss to the output.     */
	  *(.rodata)		/* Copy input section .rodata to the output.  */
}	
        . = 0x7c00 + 510;	/* Advance 510 bytes.                         */
        .signature : 		/* Add a signadure section with this content. */
	{
	    BYTE(0x55)
            BYTE(0xAA)
        }
	__END_STACK__ = 10000;	 /* End of the 64K block.        */
}
STARTUP(rt0.o)			 /* Prepend the start file.      */


/* Notes.

   BIOS shall load the program at 0x7c00.

   Our assemlber outputs an ELF object with several sections: text (executable code),
   .rodata (e.g. inline strings), .bss and .data (global variables). We need to copy
   all them into the executable file. 

   Boot signature 0xaa55 is added at position 511 to 512.

   We position the stack (bottom) at the end of the first 64K segment. Notice that
   our stack can grow downwards 64K - (0x7c00 + 512) = 32,5K.  That's a fair amount
   memory for the stack.

 */

