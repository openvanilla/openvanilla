
ECHO=/bin/echo
RM=/bin/rm -f
CP=/bin/cp
MKDIR=/bin/mkdir -p

INST_PATH=/Library/OpenVanilla/Development

INC=-I../../Headers 

CFLAGS=-O2 -Wall $(INC)

GCC=/usr/bin/gcc
GPP=/usr/bin/g++

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

