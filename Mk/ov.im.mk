CFLAGS=-O2 -Wall

GCC=/usr/bin/gcc
GPP=/usr/bin/g++

CCACHE=/usr/local/bin/ccache

ifdef USE_CCACHE
CC=$(CCACHE) $(GCC)
CPP=$(CCACHE) $(GPP)
else
CC=$(GCC)
CPP=$(GPP)
endif

VXUtility.o: ../../Loaders/OSX/VXUtility.cpp
	$(CPP) $(CFLAGS) -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

%.o: %.mm
	$(CPP) $(CFLAGS) -c -I../../Source/OSX/ -I../../Loaders/OSX $<


