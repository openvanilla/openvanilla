IMID=OVIMSocket
OBJS=OVIMSocket.o 
GOALS=OVIMSocket.dylib
DATA=OVIMsocket.pl

$(GOALS): $(OBJS)
	$(CPP) -framework Cocoa -o $@ -bundle $(OBJS)

include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk

