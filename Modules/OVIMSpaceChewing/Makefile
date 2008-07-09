# Makefile for OVIMSpaceChewing
#
# Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#	 notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#	 notice, this list of conditions and the following disclaimer in the
#	 documentation and/or other materials provided with the distribution.
# 3. Neither the name of OpenVanilla nor the names of its contributors
#	 may be used to endorse or promote products derived from this software
#	 without specific prior written permission.
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

MODULEID=OVIMSpaceChewing
SOURCES=OVIMSpaceChewing

LIBCHEWING_PREFIX?=/usr/local
LIBCHEWING_VERSION?=$(shell perl -n -e 'print "$$1\n" if /Version: (.+)/' < $(LIBCHEWING_PREFIX)/lib/pkgconfig/chewing.pc)

DATA=$(LIBCHEWING_PREFIX)/share/chewing/ch_index.dat     \
    $(LIBCHEWING_PREFIX)/share/chewing/dict.dat          \
    $(LIBCHEWING_PREFIX)/share/chewing/fonetree.dat      \
    $(LIBCHEWING_PREFIX)/share/chewing/ph_index.dat      \
    $(LIBCHEWING_PREFIX)/share/chewing/pinyin.tab        \
    $(LIBCHEWING_PREFIX)/share/chewing/swkb.dat          \
    $(LIBCHEWING_PREFIX)/share/chewing/symbols.dat       \
    $(LIBCHEWING_PREFIX)/share/chewing/us_freq.dat
    
INC+=-I$(LIBCHEWING_PREFIX)/include/chewing
CFLAGS+=-DLIBCHEWING_VERSION="\"$(LIBCHEWING_VERSION)\"" -g
LIBS+=-lchewing
LDFLAGS+=-L$(LIBCHEWING_PREFIX)/lib

include ../Mk/ov.module.mk

