OBJS=OVIMPhonetic.o vanilladata.o vanillaphone.o
GOALS=OVIMPhonetic.dylib

$(GOALS): $(OBJS)
	$(CPP) -o $@ -bundle $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk

