OBJS=OVIMXcin.o VXCIN.o VXUtility.o VXCandidate.o XcinCinList.o
GOALS=OVIMXcin.dylib 
INST_PATH=/Library/OpenVanilla/Development

all: $(GOALS)
	make -f Makefile.AddPlist

$(GOALS): $(OBJS)
	$(CPP) $(CFLAGS) -framework Cocoa -o $@ -bundle $(OBJS)

install:
	mkdir -p $(INST_PATH)
	cp $(GOALS) $(INST_PATH)
	cp *.cin $(INST_PATH)

clean:
	rm -f $(GOALS) $(OBJS)
	make -f Makefile.AddPlist clean

include ../../Mk/ov.im.mk

