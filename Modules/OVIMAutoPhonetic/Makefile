IMID=OVIMAutoPhonetic
SOURCES=OVIMAutoPhonetic PhoneticSyllable PhoneticService OVPhoneticLib
LDFLAGS=-lsqlite3
DATA=tsi.db
CINS=bpmf.cin punctuation.cin

# tsi.db: tsi-schema.txt ${CINS} tsi.src bpmf-sqlconvert.pl tsi-sqlconvert.pl
# 	rm -f tsi.db
# 	sqlite3 tsi.db < tsi-schema.txt
# 	cat ${CINS} | perl bpmf-sqlconvert.pl | sqlite3 tsi.db
# 	cat tsi.src | perl -w -CO tsi-sqlconvert.pl | sqlite3 tsi.db

include ../../Mk/ov.os.mk
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
