#!/bin/sh
SCIM_MODULEDIR=`pkg-config --variable=moduledir scim`
SCIMOV_MODULEDIR="$SCIM_MODULEDIR/scim-ov"
CIN_DIR="/tmp/OVIMArray"

libtool --mode=compile g++ -I../../Headers -c OVIMArray.cpp 
libtool --mode=compile g++ -I../../Headers -c ../../Source/OVCIN.cpp 
libtool --mode=compile g++ -I../../Headers -c ../../Source/OVCandidateList.cpp 
libtool --mode=compile g++ -I../../Headers -c ../../Source/OVFileHandler.cpp 
libtool --mode=compile g++ -I../../Headers -c ../../Source/OVStringToolKit.cpp 
libtool --mode=link g++ -module -o OVIMArray.la *.lo -rpath "$SCIMOV_MODULEDIR" -avoid-version 
mkdir -p $SCIMOV_MODULEDIR
libtool --mode=install install OVIMArray.la  "$SCIMOV_MODULEDIR"

mkdir -p $CIN_DIR
cp *.cin $CIN_DIR
