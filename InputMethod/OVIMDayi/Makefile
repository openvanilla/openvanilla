
all: OVIMDayi.dylib

OVIMDayi.dylib: OVIMDayi.o
	g++ -framework Cocoa -o $@ -bundle $<

install: OVIMDayi.dylib
	cp OVIMDayi.dylib /Library/OpenVanilla/Development/
	cp dayi3.cin /Library/OpenVanilla/Development/

clean:
	rm -f *.o
	rm -f *.dylib

%.o: %.cpp
	gcc -O2 -c $<
	
%.o: %.mm
	gcc -O2 -c $<

