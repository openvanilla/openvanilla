IMID=OVIMPOJHakka
OBJS=OVIMPOJHakka.o VXCIN.o VXUtility.o VXCandidate.o POJHakkaKeySeq.o
GOALS=OVIMPOJHakka.dylib 
DATA=pojhakka.cin

$(GOALS): $(OBJS)
	$(CPP) -framework Cocoa -o $@ -bundle $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk

