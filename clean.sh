#!/bin/sh
make clean

cd Loaders/OSX ; xcodebuild clean ; cd ../..

#cd InputMethod ;
#for directory in `ls -F | grep "\/" | sed -e 's/\///'`
#do
#   make -C $directory clean
#done
#cd ..
cd InputMethod ; make clean ; cd ..

cd Utilities/OSX/OVPrefPane  ; xcodebuild clean ; cd ../../..
