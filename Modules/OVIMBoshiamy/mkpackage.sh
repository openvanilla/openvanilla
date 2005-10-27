#!/bin/sh
##### Variables

# Only have to set this (.pkg suffix is required)
PKGNAME=OVIMBoshiamy.pkg

# Of course you'll have to install this.
PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker

##### Executives
rm -rf $PKGNAME ${PKGNAME}.tgz
sudo rm -rf root/
if [ "X$1" == "Xclean" ]; then exit; fi

mkdir root/; cd root/

tar -cf - -T ../Package/FileList --no-recursion | sudo tar -xpf -

sudo $PACKAGEMAKER -build \
  -i ../Package/Info.plist \
  -d ../Package/Description.plist \
  -p /tmp/$PKGNAME -f .

cd ..
sudo mv /tmp/$PKGNAME .
sudo chown -R $UID:staff $PKGNAME
tar -czf ${PKGNAME}.tgz ${PKGNAME}
zip ${PKGNAME}.zip `find $PKGNAME`

