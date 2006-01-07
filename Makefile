# Makefile: OpenVanilla Framework and Platform-Dependent Loader
# Copyright (c) 2004-2006 The OpenVanilla Project
# Please see License/license.txt for the terms of use

OS=`uname`

all:
	make -f Makefile.$(OS) $@

install:
	make -f Makefile.$(OS) $@

uninstall:
	make -f Makefile.$(OS) $@

clean:
	make -f Makefile.$(OS) $@

