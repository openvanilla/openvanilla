
VXUtility.o: ../../Loaders/OSX/VXUtility.cpp
	$(CPP) $(CFLAGS) -c $<

VXCIN.o: ../../InputMethod/OVIMXcin/VXCIN.mm
	$(CPP) $(CFLAGS) -c $<

VXCandidate.o: ../../InputMethod/OVIMXcin/VXCandidate.mm
	$(CPP) $(CFLAGS) -c $<

