
all: OVIMExample.dylib

OVIMExample.dylib: OVIMExample.o
	g++ -o $@ -dynamiclib $<

install: OVIMExample.dylib
	cp OVIMExample.dylib /Library/OpenVanilla/Development/
	echo "# This loads OVIMExample.dylib" > /Library/OpenVanilla/Development/OVXSimpleLoader.config
	echo "/Library/OpenVanilla/Development/OVIMExample.dylib" >> /Library/OpenVanilla/Development/OVXSimpleLoader.config

clean:
	rm /Library/OpenVanilla/Development/OVIMExample.dylib
	rm /Library/OpenVanilla/Development/OVXSimpleLoader.config

%.o: %.cpp
	gcc -c -I../ $<
	
