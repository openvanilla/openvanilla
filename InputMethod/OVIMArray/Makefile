IMID=OVIMArray
OBJS=VXCIN.o VXUtility.o VXCandidate.o OVIMArray.o 
GOALS=OVIMArray.dylib 
DATA=array30.cin

$(GOALS): $(OBJS)
	$(CPP) -framework Cocoa -o $@ -bundle $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk

# Switch the order, so that it looks up OVIMXcin.h in OSX first.
INC=-I../../Headers/OSX -I../../Headers

