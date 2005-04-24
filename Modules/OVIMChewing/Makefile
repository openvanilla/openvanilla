#CFLAGS=-O2 -Wall
#CC=g++

IMID=OVIMChewing
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk
include ../../Mk/ov.osx.mk  

OBJS=OVIMChewing.o
GOALS=OVIMChewing.dylib
INST_PATH=/Library/OpenVanilla/0.7/Modules

all: $(GOALS)

$(GOALS): $(OBJS)
	$(CPP) -framework ChewingPP -bundle -o $@ $(OBJS) 

