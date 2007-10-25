#!/bin/sh
PWD=`pwd`
DSTROOT=$PWD/Pkgroot
FRAMEWORK=$PWD/../../Framework
LOADER=$PWD/../../Loaders/OSX
MODULES=$PWD/../../Modules
OVIMSPACECHEWING=$PWD/../../Modules/OVIMSpaceChewing
OVIMUIM=$PWD/../../Modules/OVIMUIM

# sudo rm -rf $DSTROOT/*

cd $FRAMEWORK
xcodebuild clean
xcodebuild
xcodebuild DSTROOT=$DSTROOT install

cd $LOADER
xcodebuild clean
xcodebuild
xcodebuild DSTROOT=$DSTROOT install

cd $MODULES
make clean
make
make DSTROOT=$DSTROOT install

rm -rf $DSTROOT/Library/OpenVanilla/0.8/Modules/OVIMSpaceChewing*

cd $OVIMSPACECHEWING
xcodebuild clean
xcodebuild
xcodebuild DSTROOT=$DSTROOT install

cd $OVIMUIM
xcodebuild clean
xcodebuild
xcodebuild DSTROOT=$DSTROOT install
