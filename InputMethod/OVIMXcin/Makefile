OBJS=OVIMXcin.o VXCIN.o VXUtility.o VXCandidate.o XcinCinList.o
GOALS=OVIMXcin.dylib 

$(GOALS): $(OBJS)
	$(CPP) -framework Cocoa -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


