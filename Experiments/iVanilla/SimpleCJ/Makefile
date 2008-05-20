# This code is experimental and does not guarantee it will run.
# No build instruction is given. It might harm your system.
#
# Copyright (c) 2007-2008 Yao-Chung Chang and Lukhnos D. Liu
# Released under the same terms under which OpenVanilla is released 
# (The New BSD License)
#
# See LICENSE.TXT under OpenVanilla's source repository trunk/License/
# for the full text of the New BSD License.
#

CC=arm-apple-darwin-gcc
LD=$(CC)
LDFLAGS=-lobjc -lstdc++ -framework CoreFoundation -framework Foundation -framework UIKit -framework LayerKit -framework CoreGraphics -init _SimpleCJModuleInit
CFLAGS=-I/usr/local/arm-apple-darwin/include/Foundation/
all:	SimpleCJ.dylib

SimpleCJ.dylib: SimpleCJ.o MinimalLoader.o OVIMPhoneticStatic.o OVPhoneticLib.o OVPhoneticData.o
	$(LD) $(LDFLAGS) -dynamiclib -o $@ $^

%.o: %.m
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.mm
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f *.o SimpleCJ
