OBJS=OVIMCangjei.o vanillacj.o vanillasymbol.o vcdata.o vcdata_simplex.o
GOALS=OVIMCangjei.dylib

$(GOALS): $(OBJS)
	$(CPP) -o $@ -bundle $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk

