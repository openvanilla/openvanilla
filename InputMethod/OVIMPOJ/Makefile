OBJS=OVIMPOJ.o VXCIN.o VXUtility.o
GOALS=OVIMPOJ.dylib 
INST_PATH=/Library/OpenVanilla/Development

all: $(GOALS)

$(GOALS): $(OBJS)
	g++ -framework Cocoa -o $@ -bundle $(OBJS)

install:
	mkdir -p $(INST_PATH)
	cp $(GOALS) $(INST_PATH)
	cp poj.cin $(INST_PATH)

VXCIN.o: ../../Source/OSX/VXCIN.mm
	gcc -I../../Loaders/OSX -o VXCIN.o -c $<

VXUtility.o: ../../Loaders/OSX/VXUtility.cpp
	gcc -o VXUtility.o -c $<

clean:
	rm $(GOALS)

%.o: %.cpp
	gcc -O2 -c $<

%.o: %.mm
	gcc -O2 -c -I../../Source/OSX/ -I../../Loaders/OSX $<
