IMID=OVIMGenericSQLite
OBJS=OVIMGenericSQLite.o
GOALS=OVIMGenericSQLite.dylib
DATA=imtables.db
$(GOALS): $(OBJS) $(DATA)
	$(CPP) -bundle -o $@ $(OBJS) -lsqlite3

CINS=cj.cin

imtables.db: imtables-schema.txt ${CINS} cin-sqlconvert.pl
	rm -f imtables.db
	sqlite3 imtables.db < imtables-schema.txt
	perl cin-sqlconvert.pl ${CINS} | sqlite3 imtables.db

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk




