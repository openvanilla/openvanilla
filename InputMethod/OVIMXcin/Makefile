CFLAGS=-O2 -Wall
CC=g++

OBJS=OVIMXcin.o VXCIN.o VXUtility.o VXCandidate.o XcinCinList.o
GOALS=OVIMXcin.dylib 
INST_PATH=/Library/OpenVanilla/Development

all: $(GOALS)
	make -f Makefile.AddPlist

$(GOALS): $(OBJS)
	$(CC) $(CFLAGS) -framework Cocoa -o $@ -bundle $(OBJS)

install:
	mkdir -p $(INST_PATH)
	cp $(GOALS) $(INST_PATH)
	cp *.cin $(INST_PATH)

VXUtility.o: ../../Loaders/OSX/VXUtility.cpp
	$(CC) $(CFLAGS) -o VXUtility.o -c $<

clean:
	rm -f $(GOALS) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

%.o: %.mm
	$(CC) $(CFLAGS) -c -I../../Source/OSX/ -I../../Loaders/OSX $<

