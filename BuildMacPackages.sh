#!/bin/sh

if [ "x$1" == "x" ] 
then
    echo "Usage: makedist.sh version"
    exit 1
fi

PACKAGES=$PWD/build/Distributables
echo Release packages will be put in $PACKAGES
mkdir -p $PACKAGES

echo Updating loader Info.plist CFBundleVersion value to $1
$PWD/Utilities/replace-bundle-version.rb `find Loaders -name "*.plist"` $1

TSM=$PACKAGES/OpenVanilla-OSX-Tiger-$1.zip
IMK=$PACKAGES/OpenVanilla-OSX-Leopard-$1.zip


echo Removing file if it exists: $IMK
rm -f $IMK

echo Removing file if it exists: $TSM
rm -f $TSM

echo Building the targets
xcodebuild -project OpenVanilla.xcodeproj -target All -configuration Release clean build

ROOT=$PWD
cd $PWD/build/Release
zip -r $IMK OpenVanilla.app
zip -r $TSM OpenVanilla.bundle

cd $ROOT
cd $PWD/Documents
zip -r $IMK HowToInstall.txt
zip -r $TSM HowToInstall.txt

cd $ROOT

echo Release package preparation completed
