
IMID=OVIMBoshiamy
SOURCES=OVIMBoshiamy
DATA=imtables.db
CINS=liu5.cin
LDFLAGS+=-lsqlite3

include ../../Mk/ov.os.mk
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk


imtables.db: imtables-schema.txt ${CINS} cin-sqlconvert.pl
	rm -f imtables.db
	sqlite3 imtables.db < imtables-schema.txt
	perl cin-sqlconvert.pl ${CINS} | sqlite3 imtables.db


