all: OVEmu OVIMPhonetic.dylib

OVEmu: OVEmu.o VXLoadableIM.o
	g++ -o OVEmu OVEmu.o VXLoadableIM.o -ldl

OVEmu.o: OVEmu.cpp
	g++ -c -o OVEmu.o OVEmu.cpp

VXLoadableIM.o: VXLoadableIM.cpp
	g++ -c -o VXLoadableIM.o VXLoadableIM.cpp
	
OVIMPhonetic.dylib: OVIMPhonetic.cpp
	g++ -o OVIMPhonetic.so -shared OVIMPhonetic.cpp vanillaphone.cpp vanilladata.cpp


