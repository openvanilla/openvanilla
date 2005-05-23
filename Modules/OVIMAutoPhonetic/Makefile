IMID=OVIMAutoPhonetic

OBJS=OVIMAutoPhonetic.o OVPhoneticLib.o 
GOALS=OVIMAutoPhonetic.dylib
DATA=tsi.db
$(GOALS): $(OBJS) $(DATA)
	$(CPP) -bundle -o $@ $(OBJS) -lsqlite3

CINS=bpmf.cin punctuation.cin

tsi.db: tsi-schema.txt ${CINS} tsi.src bpmf-sqlconvert.pl tsi-sqlconvert.pl
	rm -f tsi.db
	sqlite3 tsi.db < tsi-schema.txt
	cat ${CINS} | perl bpmf-sqlconvert.pl | sqlite3 tsi.db
	cat tsi.src | perl -w -CO tsi-sqlconvert.pl | sqlite3 tsi.db

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk




