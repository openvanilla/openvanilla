IMID=OVIMXcin

OBJS=OVCIN.o OVCandidate.o OVFileHandler.o OVIMXcin.o\
     OVStringToolKit.o XcinCinList.o

GOALS=OVIMXcin.dylib 

DATA=cj.cin dayi3.cin jtcj_gb.cin jyutping.cin\
     phonegb2.cin pinyin.cin pinyinbig5.cin simplex.cin wubizixing.cin\
     shuangpin.cin ukiyoe.cin

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


