# An IM makefile defines OBJS and GOALS, and the direct rule
# to build GOALS from your OBJS. Other rules are deal in ov.*.mk
MODULEID=OVIMChewing03
SOURCES=OVIMChewing
OBJS=OVIMChewing.o
CFLAGS+=-I/usr/include/chewing
LDFLAGS+=-L/usr/lib -llibchewing

include ../Mk/ov.module.mk

CFLAGS+=$(INC) -g
