del *.*~
F:\mingw\bin\g++.exe -I. -I..\..\..\..\Headers -c -DWIN32 -DOV_DEBUG *.cpp .\ChewingPP\Chewing.cpp ..\..\..\..\Source\*.cpp -Wall
F:\mingw\bin\dllwrap *.o .\libchewing.lib -lstdc++ --def ..\..\OVModule.def -o OVIMChewing.dll
pause