
all: OVDayi.dylib

OVDayi.dylib: OVDayi.o
	g++ -framework Cocoa -o $@ -bundle $<

install: OVDayi.dylib
	cp OVDayi.dylib /Library/OpenVanilla/Development/
	cp dayi3.cin /Library/OpenVanilla/Development/
	echo "# This loads OVDayi.dylib" > /Library/OpenVanilla/Development/OVXSimpleLoader.config
	echo "/Library/OpenVanilla/Development/OVDayi.dylib" >> /Library/OpenVanilla/Development/OVXSimpleLoader.config

clean:
	rm *.o
	rm *.dylib
	rm /Library/OpenVanilla/Development/OVDayi.dylib
	rm /Library/OpenVanilla/Development/OVXSimpleLoader.config

%.o: %.cpp
	gcc -c -I../ $<
	
%.o: %.mm
	gcc -framework Cocoa -c -I../ $<
