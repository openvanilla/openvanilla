IMID=OVIMGeneric

OBJS=OVCIN.o OVCandidateList.o OVFileHandler.o OVIMXcin.o\
     OVStringToolKit.o XcinCinList.o

GOALS=OVIMGeneric.dylib 

DATA=cj.cin dayi3.cin iroha.cin ehq.cin

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


