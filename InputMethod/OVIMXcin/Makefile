IMID=OVIMXcin

OBJS=OVCIN.o OVCandidate.o OVFileHandler.o OVIMXcin.o\
     OVStringToolKit.o XcinCinList.o

GOALS=OVIMXcin.dylib 

DATA= cj.cin dayi3.cin jyutping.cin\
      pinyinbig5.cin simplex.cin ukiyoe.cin\
      zhcn_jtcj.cin zhcn_phonegb2.cin zhcn_pinyin.cin\
      zhcn_shuangpin.cin zhcn_wubizixing.cin

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


