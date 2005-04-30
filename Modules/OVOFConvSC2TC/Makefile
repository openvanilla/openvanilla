# An IM makefile defines OBJS and GOALS, and the direct rule
# to build GOALS from your OBJS. Other rules are deal in ov.*.mk
IMID=OVOFConvSC2TC
OBJS=OVOFConvSC2TC.o VXHCSC2TCTable.o
GOALS=OVOFConvSC2TC.dylib

$(GOALS): $(OBJS)
	$(CPP) -bundle -o $@ $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk