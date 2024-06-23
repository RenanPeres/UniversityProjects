#  Makefile - Make script.
# 
#   Copyright (c) 2021, Hiago de Franco Moreira <https://github.com/hiagofranco>
#   Copyright (c) 2021, Renan Peres Martins <https://github.com/RenanPeres>
#
#   This piece of software is a derivative work of SYSeg, by Monaco F. J.
#   SYSeg is distributed under the license GNU GPL v3, and is available
#   at the official repository https://www.gitlab.com/monaco/syseg.
#
#   This file is part of soii21-rh_OS.
#
#   soii21-rh_OS is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.





#
# Main examples and auxiliary examples
#

bin = mbr.bin

objs = main.o mbr.o

AUXDIR =../../tools#

all : $(bin) 


# Update Makefile from Makefile.m4 if needed, and then invoke make again.
# If the source is from a pack-distribution, the lack of Makefile.m4
# inhibits the updating. 

ifndef UPDATED
$(MAKECMDGOALS) : Makefile

Makefile_deps = Makefile.m4 ../../tools/docm4.m4 ../../tools/bintools.m4

Makefile : $(shell if test -f Makefile.m4; then echo $(Makefile_deps); fi);
	@if ! test -f .dist; then\
	  cd .. && make;\
	  make -f Makefile clean;\
	  make -f Makefile UPDATED=1 $(MAKECMDGOAL);\
	fi

endif


## C source code.
## We build the program using gcc, as and ld.

mbr.bin : $(objs) mbr.ld rt0.o
	ld -melf_i386 --orphan-handling=discard  -T mbr.ld $(objs) -o $@

main.o mbr.o rt0.o :%.o: %.s 
	as --32 $< -o $@

main.s mbr.s rt0.s :%.s: %.c
	gcc -m16 -O0 -I. -Wall -Wextra -fno-pic -fcf-protection=none  --freestanding -S $< -o $@

main.s mbr.s : mbr.h

#
# Housekeeping
#

clean:
	rm -f *.bin *.elf *.o *.s *.iso *.img *.i
	make clean-extra

clean-extra:
	rm -f *~ \#*



## ----------------------------------------------------------------------
##
## Programming exercise
##

PACK_FILES_C = main.c mbr.c rt0.c mbr.h mbr.ld
PACK_FILES_MAKE = Makefile
PACK_FILES_TEXT = README $(AUXDIR)/COPYING 
PACK_FILES_SH =


# Self-contained pack distribution.
#
# make pack     creates a tarball with the essential files, which can be
#      		distributed independently of the rest of this project.
#
# A pack distribution contain all the items necessary to build and run the
# relevant piece of software. It's useful,a for instance, to bundle
# self-contained subsections of SYSeg meant to be delivered as
# programming exercise assignments or illustrative source code examples.
#		
# In order to select which files should be included in the pack, list them
# in the appropriate variables
# 
# PACK_FILES_C    = C files (source, headers, linker scripts)
# PACK_FILES_MAKE = Makefiles
# PACK_FILES_TEXT = Text files (e.g. README)
# PACK_FILES_SH   = Shell scripts (standard /bin/sh)
#
# Except by text files, all other files will have their heading comment
# (the very first comment found in the file) replaced by a corresponding
# standard comments containing boilerplate copyright notice and licensing
# information, with blank fields to be filled in by the pack user.
# Attribution to SYSeg is also included for convenience.

TARNAME=mbr-0.1.1



pack:
	@if ! test -f .dist; then\
	  make do_pack;\
	 else\
	  echo "This is a distribution pack already. Nothing to be done.";\
	fi

do_pack:
	rm -rf $(TARNAME)
	mkdir $(TARNAME)
	(cd .. && make clean && make)
	for i in $(PACK_FILES_C); do\
	  cp ../../tools/c-head-pack.c $(TARNAME)/$$i ;\
	  ../../tools/stripcomment -c $$i >> $(TARNAME)/$$i;\
	done
	for i in $(PACK_FILES_MAKE); do\
	  cp ../../tools/Makefile-head-pack $(TARNAME)/$$i ;\
	  ../../tools/stripcomment -h $$i >> $(TARNAME)/$$i;\
	done
	cp $(PACK_FILES_TEXT) $(TARNAME)
	touch $(TARNAME)/.dist
	tar zcvf $(TARNAME).tar.gz $(TARNAME)

clean-pack:
	rm -f $(TARNAME).tar.gz
	rm -rf $(TARNAME)

.PHONY: pack do_pack clean-pack





# ------------------------------------------------------------
# The following excerpt of code was copied from MakeBintools,
# part of SYSeg, Copyright 2001 Monaco F. J..
# MakeBintools is a collection of handy 'GNU make' rules for
# inspecting and comparing the contents of source and object files.
# Further information: http://gitlab.com/monaco/syseg


##
## Configuration
##


# Inform your preferred graphical diff tool e.g meld, kdiff3 etc.

DIFF_TOOL=meld


##
## You probably don't need to change beyond this line
##

# Disassemble

ifndef ASM
ifeq (,$(findstring intel, $(MAKECMDGOALS)))
ASM_SYNTAX = att
else
ASM_SYNTAX = intel
endif
else
ASM_SYNTAX= $(ASM)
endif

ifndef BIT
ifeq (,$(findstring 16, $(MAKECMDGOALS)))
ASM_MACHINE = i386
else
ASM_MACHINE = i8086
endif
else

ifeq ($(BIT),16)
ASM_MACHINE = i386
else
ASM_MACHINE = i8086
endif

endif

intel att 16 32: 
	@echo > /dev/null

diss d diss* d*:  $(IMG)
	@objdump -f $< > /dev/null 2>&1; \
	if test $$? -eq 1   ; then \
	  objdump -M $(ASM_SYNTAX) -b binary -m $(ASM_MACHINE) -D $< ; \
	else \
	  if test $@ = "diss" || test $@ = "d" ; then\
	    objdump -M $(ASM_SYNTAX) -m $(ASM_MACHINE) -d $< ; \
	  else\
	    objdump -M $(ASM_SYNTAX) -m $(ASM_MACHINE) -D $< ; \
	 fi;\
	fi

%/diss %/d %/diss* %/d*: %
	make --quiet $(@F) IMG=$< $(filter 16 32 intel att, $(MAKECMDGOALS))

%/i16 %/16i : %
	make --quiet $</diss intel 16
%/i32 %/32i %/i: %
	make --quiet $</diss intel 32
%/a16 %/16a %/16 : %
	make --quiet $</diss att 16
%/a32 %/32a %/32 %/a: %
	make --quiet $</diss att 32

%/i16* %/16i* : %
	make --quiet $</diss* intel 16
%/i32* %/32i* %/i*: %
	make --quiet $</diss* intel 32
%/a16* %/16a* %/16* : %
	make --quiet $</diss* att 16
%/a32* %/32a* %/32* %/a*: %
	make --quiet $</diss* att 32

# Run on the emulator

%/run : %
	@i=$< &&\
	if test $${i##*.} = "img"; then\
	    make run-fd IMG=$<;\
	 else\
	   if test $${i##*.} = "bin"; then\
	     make run-bin IMG=$<;\
	    fi;\
	fi

%/bin : %
	make run-bin IMG=$<

%/fd : %
	make run-fd IMG=$<

# run: $(IMG)
# 	qemu-system-i386 -drive format=raw,file=$< -net none


# Dump contents in hexadecimal

dump: $(IMG)
	hexdump -C $<


%/dump : %
	make --quiet dump IMG=$< 


# Diff-compare


MISSING_DIFF_TOOL="Can't find $(DIFF_TOOL); please edit syseg/tools/makefile.utils"

objdiff bindiff : $(wordlist 2, 4, $(MAKECMDGOALS))
	if  test -z $(which $(DIFF_TOOL)); then echo $(MISSING_DIFF_TOOL); exit 1; fi
	if test $(words $^) -lt 3 ; then\
	  bash -c "$(DIFF_TOOL) <(make $(wordlist 1,1,$^)/diss) <(make $(wordlist 2,2,$^)/diss)";\
	else\
	  bash -c "$(DIFF_TOOL) <(make $(wordlist 1,1,$^)/diss) <(make $(wordlist 2,2,$^)/diss) <(make $(wordlist 3,3,$^)/diss)";\
	fi

srcdiff : $(wordlist 2, 4, $(MAKECMDGOALS))
	if  test -z $(which $(DIFF_TOOL)); then echo $(MISSING_DIFF_TOOL); exit 1; fi
	if test $(words $^) -lt 3 ; then\
	  bash -c "$(DIFF_TOOL) $(wordlist 1,1,$^) $(wordlist 2,2,$^)";\
	else\
	  bash -c "$(DIFF_TOOL) $(wordlist 1,1,$^) $(wordlist 2,2,$^) $(wordlist 3,3,$^)";\
	fi

diff : $(word 2, $(MAKECMDGOALS))
	@echo $(wordlist 2, 4, $(MAKECMDGOALS))
	@EXT=$(suffix $<);\
	case $$EXT in \
	.bin | .o)\
		make --quiet objdiff $(wordlist 2, 4, $(MAKECMDGOALS))\
		;;\
	.asm | .S.| .s | .i | .c | .h | .hex)\
		make --quiet srcdiff $(wordlist 2, 4, $(MAKECMDGOALS))\
		;;\
	*)\
		echo "I don't know how to compare filetype $$EXT"\
		;;\
	esac



##
## Create bootable USP stick if BIOS needs it
##

%.iso : %.img
	xorriso -as mkisofs -b $< -o $@ -isohybrid-mbr $< -no-emul-boot -boot-load-size 4 ./

%.img : %.bin
	dd if=/dev/zero of=$@ bs=1024 count=1440
	dd if=$< of=$@ seek=0 conv=notrunc

run-bin: $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none

run-iso: $(IMG)
	qemu-system-i386 -drive format=raw,file=$(IMG) -net none

run-fd : $(IMG)
	qemu-system-i386 -drive if=floppy,format=raw,file=$< -net none


stick: $(IMG)
	@if test -z "$(STICK)"; then \
	echo "*** ATTENTION: make IMG=foo.bin SITCK=/dev/X"; exit 1; fi 
	dd if=$< of=$(STICK)

.PHONY: clean clean-extra intel att 16 32 diss /diss /i16 /i32 /a16 /a32


# End of MakeBintools.
# -------------------------------------------------------------


