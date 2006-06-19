CPPFLAGS=-Wall

all: OVTester
	@./OVTester

clean:
	@rm -f OVTester *~ *.o

OVTester: OVTester.cpp
	g++ $(CPPFLAGS) -ltap -lpthread -o $@ $<
