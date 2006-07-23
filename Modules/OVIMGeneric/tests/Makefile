
CPPFLAGS=-Wall -I/usr/local/include -L/usr/local/lib -ltap 
OS=$(shell uname)

ifeq "$(OS)" "FreeBSD"
CPPFLAGS+= -pthread
endif

all: OVTester
	@./OVTester

clean:
	@rm -f OVTester *~ *.o

OVTester: OVTester.cpp
	g++ $(CPPFLAGS) -o $@ $<
