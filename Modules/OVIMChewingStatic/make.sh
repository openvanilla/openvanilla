#!/bin/sh

gcc -I./ -I./libchewing -c libchewing/char.c libchewing/chewingio.c libchewing/chewingutil.c libchewing/choice.c libchewing/key2pho.c libchewing/dict.c libchewing/hanyupinying.c libchewing/hash.c libchewing/tree.c libchewing/userphrase.c libchewing/zuin.c

g++ -I./ -I./libchewing/ -bundle -o OVIMChewingStatic.dylib  Chewingpp.cpp OVIMChewingStatic.cpp  *.o

# sudo cp OVIMChewingStatic.dylib /Library/OpenVanilla/0.7.0/Modules/
