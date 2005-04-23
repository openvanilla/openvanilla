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
	$(CC) -framework ChewingPP -o $@ -bundle $<

#install: $(GOALS) 
#	mkdir -p $(INST_PATH)
#	cp $(GOALS) $(INST_PATH)

#clean:
#	rm -rf $(GOALS) $(OBJS) OVIMChewing.pkg OVIMChewing.pkg.tgz
#	./mkpackage.sh clean

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

package: force
	./mkpackage.sh

force: ;

