IMID=OVIMPhoneticSQLite

DB=bpmf.db
OBJS=OVIMPhoneticSQLite.o OVPhoneticLib.o 
GOALS=OVIMPhoneticSQLite.dylib
DATA=bpmf.db
$(GOALS): $(OBJS) $(DB)
	$(CPP) -bundle -o $@ $(OBJS) -lsqlite3

CINS=bpmf.cin punctuation.cin

bpmf.db: bpmf-schema.txt ${CINS} bpmf-sqlconvert.pl
	rm -f bpmf.db
	sqlite3 bpmf.db < bpmf-schema.txt
	cat ${CINS} | perl bpmf-sqlconvert.pl | sqlite3 bpmf.db

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk




