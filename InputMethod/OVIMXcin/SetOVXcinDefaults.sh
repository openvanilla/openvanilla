#!/bin/sh
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-array30.cin autoCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-cj.cin autoCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-simplex.cin hitMaxAndCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-simplex.cin maxKeySequenceLength integer 2
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-dayi3.cin shiftSelectionKey integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-pinyinbig5.cin maxKeySequenceLength integer 10
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-zhcn_jtcj.cin autoCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-zhcn_pinyin.cin maxKeySequenceLength integer 10
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-bopomofo.cin maxKeySequenceLength integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-bopomofo.cin hitMaxAndCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-liu57.cin maxKeySequenceLength integer 4 
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-liu57.cin autoCompose integer 1 
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-liu57.cin hitMaxAndCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-ehq.cin maxKeySequenceLength integer 6
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-ukiyoe.cin maxKeySequenceLength integer 4
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-kk.cin maxKeySequenceLength integer 2
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-kk.cin hitMaxAndCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-klingon.cin maxKeySequenceLength integer 1 
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-klingon.cin autoCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-klingon.cin hitMaxAndCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist OVLoader currentIM string OVIMPhonetic
chmod 666 /Library/OpenVanilla/0.6.2/OVLoader.plist
