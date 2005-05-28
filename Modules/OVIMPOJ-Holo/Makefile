IMID=OVIMPOJ-Holo

OBJS=OVCIN.o OVCandidateList.o OVFileHandler.o OVStringToolKit.o \
	OVIMPOJ-Holo.o POJ-Holo.o

GOALS=OVIMPOJ-Holo.dylib 

DATA=poj-holo.cin

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


