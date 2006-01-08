# Each Inputmethod should define a IMID as it's identifier.
# It'll be used as a part of data installation path.

ECHO=/bin/echo
RM=/bin/rm -f
CP=/bin/cp
MKDIR=/bin/mkdir -p

DATA_INST_PATH?=$(INST_PATH)/$(IMID)

INC=-I../../Headers -I/usr/local/include


GCC=/usr/bin/gcc 
GPP=/usr/bin/g++
LIBTOOL=$(shell ../../Utilities/find-libtool.pl)


ifeq ("$(OS)", "Darwin")
    ARCH=-arch i386 -arch ppc 
    SDK= /Developer/SDKs/MacOSX10.4u.sdk
    CFLAGS+= $(ARCH) -O2 -Wall $(INC) -isysroot ${SDK} -F/Library/Frameworks/ 
    LDFLAGS+= $(ARCH) -isysroot ${SDK}
else
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

VPATH+= ../../Source/
DATA?=

