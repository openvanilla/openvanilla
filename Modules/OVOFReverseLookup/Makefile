IMID=OVOFReverseLookup

OBJS=OVCIN.o OVCandidateList.o OVFileHandler.o OVOFReverseLookup.o \
     OVStringToolKit.o XcinCinList.o

GOALS=OVOFReverseLookup.dylib 

DATA=cj-lookup.cin phonetic-lookup.cin poj-lookup.cin

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


