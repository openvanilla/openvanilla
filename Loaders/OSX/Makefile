# Makefile: xcodebuild wrapper for OpenVanilla Loader for Mac OS X
# Copyright (c) 2004-2007 The OpenVanilla Project
# Please see trunk/License/license.txt for the terms of use

BUILDER=xcodebuild
CONFIG?=Release
DSTROOT?=/
CMDLINETOOLROOT?=/usr/local

PROJECT=OpenVanillaLoader.xcodeproj

PRODUCTS=$(DSTROOT)/Library/Components/OpenVanilla-TSComponent-0.8.0.bundle \
	$(DSTROOT)/Library/OpenVanilla/0.8.0/Loader \
	$(DSTROOT)/$(CMDLINETOOLROOT)/bin/ovfilter

all:
	xcodebuild -project $(PROJECT) -configuration $(CONFIG) build $(MAKEFLAGS)

install:
	xcodebuild -project $(PROJECT) -configuration $(CONFIG) install $(MAKEFLAGS)

clean:
	xcodebuild -project $(PROJECT) -configuration $(CONFIG) clean $(MAKEFLAGS)

uninstall:
	rm -rf $(PRODUCTS)
	
