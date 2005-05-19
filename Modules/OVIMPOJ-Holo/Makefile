IMID=OVIMPOJ-Holo
OBJS=OVIMPOJ.o VXCIN.o VXUtility.o VXCandidate.o POJKeySeq.o
GOALS=OVIMPOJ-Holo.dylib 
DATA=poj.cin

$(GOALS): $(OBJS)
	$(CPP) -framework Cocoa -o $@ -bundle $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk

