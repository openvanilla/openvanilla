#!/bin/sh
cd ../Modules/OVIMChewing
make install clean

cp -r /usr/local/share/chewing/* /Library/OpenVanilla/0.7.0/Modules/OVIMChewing/

# prepare everything under /tmp/OV070rc4PackageRoot
PKGROOT=/tmp/OV070rc4PackageRoot
mkdir -p $PKGROOT/Library/OpenVanilla/0.7.0/Modules/OVIMChewing
mkdir -p $PKGROOT/Library/Frameworks/

cp -r /usr/local/lib/libchewing.2.dylib $PKGROOT/usr/local/lib
cp -r /Library/Frameworks/ChewingPP.framework $PKGROOT/Library/Frameworks/
cp -r /Library/OpenVanilla/0.7.0/Modules/OVIMChewing* $PKGROOT/Library/OpenVanilla/0.7.0/Modules/

