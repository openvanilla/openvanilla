#!/bin/sh
# OV Packaging script
# Author: Kang-min "Gugod" Liu <gugod@gugod.org>

# This tool build OpenVanilla package.
# Provided that you install ChewingPP and libchewing.
# You'll have to be a sudoer to use this script.
# install OpenVanilla with f*cked permission and owner.
# The files in Package/FileList are automatically set to
# the same permission on your files system.
# WARNING: You'll have to add parent directories in FileList otherwise
# directory permissions will be wrong.

##### Variables

# Only have to set this (.pkg suffix is required)
PKGNAME=OpenVanilla.pkg

# Of course you'll have to install this.
PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker

##### Executives
rm -rf $PKGNAME ${PKGNAME}.tgz
sudo rm -rf root/
if [ "X$1" == "Xclean" ]; then exit; fi

mkdir root/; cd root/


tar -cf - -T ../FileList --no-recursion | sudo tar -xpf -

sudo $PACKAGEMAKER -build \
  -i ../Info.plist \
  -d ../Description.plist \
  -r ../Resource \
  -p /tmp/$PKGNAME -f .

cd ..
sudo mv /tmp/$PKGNAME .
sudo chown -R $UID:staff $PKGNAME
tar -czf ${PKGNAME}.tgz ${PKGNAME}

