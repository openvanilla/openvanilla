#!/bin/sh
cd ..

# build framework
make install clean

# build Loader
cd Loaders/OSX
make install clean

# build bundled modules
cd ../../Modules/OVIMArray
make install clean
cd ../../Modules/OVIMGeneric
make install clean
cd ../../Modules/OVIMPOJ-Holo
make install clean
cd ../../Modules/OVIMPhonetic
make install clean
cd ../../Modules/OVIMRoman
make install clean
cd ../../Modules/OVIMTibetan
make install clean
cd ../../Modules/OVOFAntiZhuyinwen
make install clean
cd ../../Modules/OVOFConvSC2TC
make install clean
cd ../../Modules/OVOFConvTC2SC
make install clean
cd ../../Modules/OVOFFullWidthCharacter
make install clean
cd ../../Modules/OVOFReverseLookup
make install clean
cd ../../

# prepare everything under /tmp/OV070rc4PackageRoot
PKGROOT=/tmp/OV070rc4PackageRoot
mkdir -p $PKGROOT/usr/local/lib
mkdir -p $PKGROOT/Library/Components
mkdir -p $PKGROOT/Library/OpenVanilla/0.7.0
mkdir -p $PKGROOT/Library/Frameworks/
cp -r /Library/Components/OVInit.bundle $PKGROOT/usr/Library/Components
cp -r /Library/OpenVanilla/0.7.0 $PKGROOT/Library/OpenVanilla/0.7.0
cp -r /Library/Frameworks/OpenVanilla.framework $PKGROOT/Library/Frameworks/
