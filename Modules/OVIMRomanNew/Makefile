# An IM makefile defines OBJS and GOALS, and the direct rule
# to build GOALS from your OBJS. Other rules are deal in ov.*.mk
MODULEID=OVIMRomanNew
SOURCES=OVIMRomanNew
DATA=dict.db

include ../Mk/ov.module.mk

LDFLAGS+=-lsqlite3 -L/usr/local/lib -L/opt/local/lib -laspell
INC+=-I/usr/local/include -I/opt/local/include
CFLAGS=-g $(INC)

all:	OVIMRomanNew.dylib $(DATA)

dict.db: cin-sqlconvert.pl
	rm -f dict.db
	perl cin-sqlconvert.pl dict.txt | sqlite3 dict.db

