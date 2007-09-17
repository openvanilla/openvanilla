# ov.module.mk: OpenVanilla Module Makefile Header
#
# Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of OpenVanilla nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

#
#
# What to put in your own Makefile:
#
# MODULEID=name_of_the_library (e.g. MODULEID=OVIMExample => OVIMExample.dylib)
# DATA=data_files_to_be_copied
# SOURCES=source_files      (without .c .cpp .m .mm extensions)
# EXTRA_GOALS=extra_goals   (put the extra target names here)
# EXTRA_CLEAN=other_files_to_be_cleaned
# HAS_VARIANTS=variant_names
#
# Then, in the end, insert a line "include ../Mk/ov.module.mk", and
# this will do the magic for you!
#
#

###### Per-Version Settings

# Use DSTROOT on Darwin (Mac OS X), 
# INSTALL_PREFIX and INSTALL_LIBPREFIX on everything else
DSTROOT?=/
INSTALL_PREFIX?=/usr/local/
INSTALL_LIBPREFIX?=/lib/
OV_INCLUDE=/usr/local/include

BUILDER=xcodebuild
CONFIG?=Release

###### OS Settings

OS?=$(shell uname)

OS_COMPILE?=$(OS)
OS_INSTALL?=$(OS)

# install rules for Darwin (Mac OS X)
ifeq ("$(OS_INSTALL)", "Darwin")
    INST_PATH=$(DSTROOT)/Library/OpenVanilla/0.8.0/Modules/
	OBJS=$(patsubst %, %.o, $(SOURCES))
	GOALS=$(MODULEID).dylib

$(GOALS): $(OBJS) $(EXTRA_GOALS)
	$(CPP) -bundle $(LDFLAGS) $(LIBS) -o $@ $(OBJS)

install-goal:
	$(CP) $(GOALS) $(INST_PATH)

uninstall-goal:
	for i in $(GOALS) ; do rm -rf $(INST_PATH)/$$i ; done

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.m
	$(CC) $(CFLAGS) -c $<

%.o: %.mm
	$(CPP) $(CFLAGS) -c $<

# install rules for everthing else
else
    # default for INST_PATH is /usr/local/lib/openvanilla
	INST_PATH=$(INSTALL_PREFIX)/$(INSTALL_LIBPREFIX)/openvanilla/
	OBJS=$(patsubst %, %.lo, $(SOURCES))
	GOALS=$(MODULEID).la
	#CFLAGS=-I/usr/local/include/ -I../../Headers/

$(GOALS): $(OBJS) $(EXTRA_GOALS)
	$(LIBTOOL) --tag=CXX --mode=link $(CPP) $(LDFLAGS) $(LIBS) -module -rpath $(INST_PATH) -avoid-version -o $(GOALS)  $(OBJS)

install-goal:
	$(MKDIR) $(INST_PATH)
	$(LIBTOOL) --mode=install install $(GOALS) $(INST_PATH)

uninstall-goal:
	rm -rf $(INST_PATH)/$(MODULE_ID)*

%.lo: %.cpp
	$(LIBTOOL) --mode=compile --tag=CXX $(CPP) $(CFLAGS) -c $<

%.lo: %.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(CFLAGS) -c $<

endif


###### Module-Target Settings

all: build ;

build: $(GOALS) ;

install: all install-mkdir install-goal install-data ;

uninstall: uninstall-goal uninstall-data uninstall-rmdir;

install-mkdir:
	$(MKDIR) $(INST_PATH)
	$(MKDIR) $(DATA_INST_PATH)

install-data:
	if [ "$(DATA)" ]; then for i in $(DATA); do $(CP) $$i $(DATA_INST_PATH); done; fi

uninstall-rmdir:
	$(RMDIR) $(DATA_INST_PATH)
    
uninstall-data:
	if [ "$(DATA)" ]; then for i in $(DATA); do rm -rf $(DATA_INST_PATH)/$$i; done; fi

clean:
	$(RM) $(GOALS) $(EXTRA_CLEAN) $(EXTRA_GOALS) $(OBJS)

help:
	@echo "This is the Makefile for $(MODULEID). Available targets:"
	@echo "    build              Builds the module"
	@echo "    install            Install the module (may require sudo)"
	@echo "    clean              Clean up built files"
	@echo "    help               What you're reading"
	@echo "Customizable Makefile variables:"
	@echo "    DSTROOT            Install root for Darwin/Mac OS X (default=/)"
	@echo "    INSTALL_PREFIX     Install prefix for other platforms (default=/usr/local/)"
	@echo "    INSTALL_LIBPREFIX  Library prefix (default=/lib/)"
	@echo "    OV_INCLUDE         Search path for OV framework (default=/usr/local/include)"
	@echo "    OS                 Target platform (default=$(shell uname))"
	@echo "    OS_COMPILE         Target platform at compiling stage (default=OS)"
	@echo "    OS_INSTALL         Target platform at installation stage (default=OS)"
	@echo "    VARIANT            Build the variant (if any)"
ifdef HAS_VARIANTS
	@echo "This module has the following variant(s):"
	@echo "    $(HAS_VARIANTS)"
else
	@echo "This modules does not have any variant."
endif    

bundle:
	$(BUILDER) -project $(PROJECT) -configuration $(CONFIG) build $(MAKEFLAGS)

###### Variables

# In the Makefile of each module, there should be a MODULEID defined
# This is used as the name of the directory to which data is copied

ECHO=/bin/echo
RM=/bin/rm -f
RMDIR=/bin/rm -rf
CP=/bin/cp
MKDIR=/bin/mkdir -p

DATA_INST_PATH?=$(INST_PATH)/$(MODULEID)

GCC=/usr/bin/gcc 
GPP=/usr/bin/g++
LIBTOOL=$(shell ../Mk/find-libtool.pl)


ifeq ("$(OS_COMPILE)", "Darwin")
    override INC+=-I../SharedHeaders
    ARCH=-arch i386 -arch ppc 
    SDK=/Developer/SDKs/MacOSX10.4u.sdk
    override CFLAGS+=$(ARCH) -O2 -Wall $(INC) -isysroot ${SDK} \
        -F/Library/Frameworks/
    override LDFLAGS+= $(ARCH) -isysroot ${SDK}
else
    INC+=-I../SharedHeaders -I$(OV_INCLUDE)
    CFLAGS+=-O2 -Wall $(INC)
endif


CCACHE=ccache
ifdef USE_CCACHE
    CC=$(CCACHE) $(GCC)
    CPP=$(CCACHE) $(GPP)
else
    CC=$(GCC)
    CPP=$(GPP)
endif

VPATH+=../SharedSource/
DATA?=

