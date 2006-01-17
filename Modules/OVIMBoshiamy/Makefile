# Makefile for OVIMBoshiamy

MODULEID=OVIMBoshiamy
SOURCES=OVIMBoshiamy
DATA=imtables.db
CINS=liu5.cin
LDFLAGS+=-lsqlite3
EXTRA_GOALS=imtables.db

include ../Mk/ov.module.mk

imtables.db: imtables-schema.txt ${CINS} cin-sqlconvert.pl
	rm -f imtables.db
	sqlite3 imtables.db < imtables-schema.txt
	perl cin-sqlconvert.pl ${CINS} | sqlite3 imtables.db
