all: OVEmu

OVEmu: OVEmu.o VXLoadableIM.o
	g++ -o OVEmu OVEmu.o VXLoadableIM.o

OVEmu.o: OVEmu.cpp
	g++ -c -I../OSX/ -o OVEmu.o OVEmu.cpp

VXLoadableIM.o: ../OSX/VXLoadableIM.cpp
	g++ -c -I../OSX/ -o VXLoadableIM.o ../OSX/VXLoadableIM.cpp

