#!/bin/sh

make; sudo make install

cd Loaders/OSX ; make ; sudo make install; cd ../..

cd InputMethod ; make ; sudo make install; cd ..

cd InputMethod/OVIMXcin ; make -f Makefile.AddPlist ; sudo ./SetOVXcinDefaults.sh ; cd ../..

cd Utilities/OSX/OVPrefPane ; make ; sudo make install ; cd ../../..

cd Documents ; sudo make install ; cd ..

cd InputMethodExtra/OVIMXcinExtra; sudo make install ; cd ../..
