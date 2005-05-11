# An IM makefile defines OBJS and GOALS, and the direct rule
# to build GOALS from your OBJS. Other rules are deal in ov.*.mk
IMID=OVIMArray
OBJS=OVIMArray.o OVCIN.o OVStringToolKit.o OVFileHandler.o OVCandidateList.o
GOALS=OVIMArray.dylib
DATA=ArrayShortCode.cin array30.cin

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk


