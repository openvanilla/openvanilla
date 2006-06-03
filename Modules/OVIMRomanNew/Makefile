# An IM makefile defines OBJS and GOALS, and the direct rule
# to build GOALS from your OBJS. Other rules are deal in ov.*.mk
IMID=OVIMRomanNew
SOURCES=OVIMRomanNew
DATA=dict.db

include ../../Mk/ov.os.mk
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk

LDFLAGS+=-lsqlite3 -L/usr/local/lib -laspell
INC+=-I/usr/local/include
CFLAGS=-g $(INC)

all:	OVIMRomanNew.dylib $(DATA)

dict.db: cin-sqlconvert.pl
	rm -f dict.db
	perl cin-sqlconvert.pl dict.txt | sqlite3 dict.db

