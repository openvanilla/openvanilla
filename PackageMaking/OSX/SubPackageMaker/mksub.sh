#!/bin/sh
# OV Packaging script for making subpackages
# Based on gugod's mkpackage.sh

# Only have to set this (.pkg suffix is required)
PKGNAME=$1

# Of course you'll have to install this.
PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker

##### Executives
if [ "X$1" == "X" ]; then echo "usage: mksub packagename"; exit; fi


rm -rf $PKGNAME
sudo rm -rf root/
if [ "X$1" == "Xclean" ]; then exit; fi

mkdir root/; cd root/


tar -cf - -T ../FileList --no-recursion | sudo tar -xpf -

sudo $PACKAGEMAKER -build \
  -i ../Info.plist \
  -r ../Resource \
  -d ../Description.plist \
  -p /tmp/$PKGNAME -f . 

cd ..
sudo mv /tmp/$PKGNAME .
sudo chown -R $UID:staff $PKGNAME
sudo rm -rf root/

