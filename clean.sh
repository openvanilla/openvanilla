#!/bin/sh
/usr/bin/make clean

cd Experiments/OVPrefPane ; xcodebuild clean ; cd ../..

cd Loaders/OSX ; xcodebuild clean ; cd ..

cd InputMethod ;
for directory in `ls -F | grep "\/" | sed -e 's/\///'`
do
   make -C $directory clean
done
cd ..
