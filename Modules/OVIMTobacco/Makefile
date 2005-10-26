IMID=OVIMTobacco
SOURCES=OVIMTobacco PredictorSingleton DictionarySingleton\
    Token Vocabulary Candidate BiGram
DATA=imtables.db tsi.db
BPMF=bpmf_punctuation.cin
CINS=cj.cin dayi3.cin poj-holo.cin

LDFLAGS+=-lsqlite3

include ../../Mk/ov.os.mk
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk

INC+=-I../../Experiments/Tobacco
VPATH+= ../../Experiments/Tobacco
CFLAGS=-g $(INC)

all: OVIMTobacco.dylib $(DATA)

imtables.db: imtables-schema.txt ${BPMF} ${CINS} cin-sqlconvert.pl
	rm -f imtables.db
	sqlite3 imtables.db < imtables-schema.txt
	perl cin-sqlconvert.pl ${BPMF} ${CINS} | sqlite3 imtables.db
	
tsi.db: tobacco-schema.txt tobacco-sqlconvert.pl
	rm -f tsi.db
	sqlite3 tsi.db < tobacco-schema.txt
	if [ !tsi.src ]; then curl -k https://svn.csie.net/chewing/libchewing/trunk/data/tsi.src -o tsi.src ; fi
	cat tsi.src | perl -w -CO tobacco-sqlconvert.pl | sqlite3 tsi.db
	perl -w -CO tobacco-sqlconvert_general.pl tsi.src ${CINS} | sqlite3 tsi.db