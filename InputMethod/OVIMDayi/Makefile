
all: OVIMDayi.dylib

OVIMDayi.dylib: OVIMDayi.o
	g++ -framework Cocoa -o $@ -bundle $<

install: OVIMDayi.dylib
	cp OVIMDayi.dylib /Library/OpenVanilla/Development/
	cp dayi3.cin /Library/OpenVanilla/Development/
	echo "# This loads OVIMDayi.dylib" > /Library/OpenVanilla/Development/OVXSimpleLoader.config
	echo "/Library/OpenVanilla/Development/OVIMDayi.dylib" >> /Library/OpenVanilla/Development/OVXSimpleLoader.config
#	cp dayi3.cin /Library/OpenVanilla/Development/

clean:
	rm *.o
	rm *.dylib
	rm /Library/OpenVanilla/Development/OVIMDayi.dylib
	rm /Library/OpenVanilla/Development/OVXSimpleLoader.config

%.o: %.cpp
	gcc -c $<
	
%.o: %.mm
	gcc -c $<
