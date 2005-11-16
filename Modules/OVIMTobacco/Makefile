IMID=OVIMTobacco
SOURCES=OVIMTobacco PredictorSingleton DictionarySingleton\
    Token Vocabulary Candidate BiGram
DATA=imtables.db tsi.db
BPMF=bpmf_punctuation.cin
CINS=cj.cin

LDFLAGS+=-lsqlite3

include ../../Mk/ov.os.mk
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk

INC+=-I../../Experiments/Tobacco
VPATH+= ../../Experiments/Tobacco
CFLAGS+=$(INC) -g

all: OVIMTobacco.dylib $(DATA)

imtables.db: imtables-schema.txt ${BPMF} ${CINS} cin-sqlconvert.pl
	rm -f imtables.db
	sqlite3 imtables.db < imtables-schema.txt
	perl cin-sqlconvert.pl ${BPMF} ${CINS} | sqlite3 imtables.db
	
tsi.db: tobacco-schema.txt tobacco-sqlconvert.pl
	rm -f tsi.db
	sqlite3 tsi.db < tobacco-schema.txt
	svn cat https://svn.csie.net/chewing/libchewing/branches/utf8/data/tsi.src > tsi.src
	patch tsi.src tsi.src.patch
	cat tsi.src punctuation.src > tsi_punctuation.src
	perl -w -CO tobacco-sqlconvert.pl tsi_punctuation.src | sqlite3 tsi.db
	perl -w -CO tobacco-sqlconvert_general.pl tsi_punctuation.src ${CINS} | sqlite3 tsi.db
