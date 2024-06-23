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
#  	soii21-rh_OS. is free software: you can redistribute it and/or modify
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





## 
## Stage 1: boot loader
## Stage 2: kernel
##

bin = stage1.bin stage2.bin

# Stage 1 uses some basic functions implemented in core.c

stage1_obj = stage1.o core.o

# Stage 2 also uses core.c, but includes a lot more implemented in tyos.c

stage2_obj = stage2.o core.o tyos.o

# Size of stage2 in 512-byte sectors

STAGE2_SIZE= 10

# Auxiliary variables to simplify this Makefile

all_obj = $(sort $(stage1_obj) $(stage2_obj)) rt0.o

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



###########################################################
##
## These are the rules of interest in this set of examples.
##



## C source code.
## We build the program step by step so that we can inspect the intrmediate
## assembly, object and binaries if we want to.

stage1.bin : $(stage1_obj) stage1.ld rt0.o
	ld -melf_i386 --orphan-handling=discard  -T stage1.ld $(stage1_obj) -o $@

stage2.bin : $(stage2_obj) stage2.ld
	ld -melf_i386 --orphan-handling=discard  -T stage2.ld $(stage2_obj) -o $@

$(all_obj) :%.o: %.s 
	as --32 $< -o $@

$(all_obj:%.o=%.s) :%.s: %.c
	gcc -m16 -O0 -I. -Wall -Wextra -fno-pic -fcf-protection=none  --freestanding -S $< -DSTAGE2_SIZE=$(STAGE2_SIZE) -o $@

$(stage1_obj:%.o=%.s) $(stage2_obj:%.o=%.s) : tyos.h

## Here we create a 1.44M floppy disk image.
## We store stage1.bin at the MBR area (first 512 bytes), and
## stage1.bin right after it. 

tyos.img: stage1.bin stage2.bin
	rm -f $@
	dd              bs=1440 count=1024               if=/dev/zero of=$@
	dd conv=notrunc bs=512  count=1 obs=512 seek=0 if=stage1.bin of=$@
	dd conv=notrunc bs=512  count=$(STAGE2_SIZE) obs=512 seek=1 if=stage2.bin of=$@


#
# Housekeeping
#

clean:
	rm -f *.bin *.elf *.o *.s *.iso *.img *.i
	make clean-extra

clean-extra:
	rm -f *~ \#*


#
# Programming exercise
#

PACK_FILES_C = core.c  rt0.c  stage1.c  stage2.c  tyos.c stage1.ld stage2.ld tyos.h
PACK_FILES_MAKE = Makefile
PACK_FILES_TEXT = README ../../tools/COPYING 
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

TARNAME=tyos-0.1.0



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



# Include Make Bintools

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


