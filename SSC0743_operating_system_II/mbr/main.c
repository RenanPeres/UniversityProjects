/* main.c - Prompt looping source file
 
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

#include <mbr.h>

int main()
{
  char cmd[BUFFER_MAX_LENGTH];

  /* Clear screen. */
  
  clear();

  /* Main loop. */
  
  while (1)
    {
  
      print (PROMPT);		                                  /* Show prompt. */

      read (cmd);		                                      /* Read user command. */

      /* Process user command. */
      if (compare(cmd, TIME_CMD)){time(); print(nl);}     /* Command time */
//      else if (compare(cmd, HELP_CMD)) help();          /* Command help. */
      else if (compare(cmd, QUIT_CMD))	quit();           /* Command quit. */
      else{
        print (cmd);		                                  /* Unkown command. */
        printnl (NOT_FOUND);
	    }
    }
  
  return 0;

}
