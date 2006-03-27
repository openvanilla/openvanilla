
MODULEID=OVIMGenericSQLite
SOURCES=OVIMGenericSQLite
DATA=imtables.db
CINS=ehq-symbols.cin

LDFLAGS+=-lsqlite3

include ../Mk/ov.module.mk

imtables.db: imtables-schema.txt ${CINS} cin-sqlconvert.pl
	rm -f imtables.db
	sqlite3 imtables.db < imtables-schema.txt
	perl cin-sqlconvert.pl ${CINS} | sqlite3 imtables.db


