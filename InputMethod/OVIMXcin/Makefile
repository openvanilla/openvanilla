OBJS=OVCIN.o OVCandidate.o OVFileHandler.o OVIMXcin.o OVStringToolKit.o XcinCinList.o

GOALS=OVIMXcin.dylib 

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


