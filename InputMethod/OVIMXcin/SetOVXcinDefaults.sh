#!/bin/sh
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-array30.cin autoCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-cj.cin autoCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-jtcj_gb.cin autoCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-simplex.cin hitMaxAndCompose integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-simplex.cin maxKeySequenceLength integer 2
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-dayi3.cin shiftSelectionKey integer 1
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-pinyin.cin maxKeySequenceLength integer 10
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist IM-OVIMXcin-pinyinbig5.cin maxKeySequenceLength integer 10
./AddPlist /Library/OpenVanilla/0.6.2/OVLoader.plist OVLoader currentIM string OVIMPhonetic
chmod 666 /Library/OpenVanilla/0.6.2/OVLoader.plist
