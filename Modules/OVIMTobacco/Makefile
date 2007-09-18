MODULEID=OVIMTobacco
SOURCES=OVIMTobacco PredictorSingleton DictionarySingleton\
    Token Vocabulary Candidate BiGram\
    ProfileManager ProfileFetcher Cache Profile
DATA=imtables.db tsi.db
BPMF=bpmf_punctuation.cin bpmf-Hsu.cin
CINS=../SharedData/cj.cin

LDFLAGS+=-lsqlite3

INC+=-I../../Experiments/Tobacco -I../SharedHeaders
VPATH+= ../../Experiments/Tobacco ../SharedSource

include ../Mk/ov.module.mk

CFLAGS+=$(INC) -g

#all: OVIMTobacco.dylib $(DATA)

imtables.db: imtables-schema.txt ${BPMF} ${CINS} cin-sqlconvert.pl
	rm -f imtables.db
	sqlite3 imtables.db < imtables-schema.txt
	perl cin-sqlconvert.pl ${BPMF} ${CINS} | sqlite3 imtables.db
	
tsi.db: tobacco-schema.txt tobacco-sqlconvert.pl
	rm -f tsi.db
	sqlite3 tsi.db < tobacco-schema.txt
	cat iasl.lm punctuation.lm > tobacco.lm
	perl -w -CO tobacco-sqlconvert.pl tobacco.lm | sqlite3 tsi.db
	perl -w -CO tobacco-sqlconvert_general.pl tobacco.lm ${CINS} | sqlite3 tsi.db
