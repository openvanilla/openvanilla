del *.*~
F:\mingw\bin\g++.exe -I. -I..\..\..\..\Framework\Headers -I..\..\..\..\Modules\SharedHeaders -c -DWIN32 -DOV_DEBUG *.cpp .\ChewingPP\Chewing.cpp ..\..\..\..\Modules\SharedSource\*.cpp -Wall
F:\mingw\bin\dllwrap *.o .\libchewing.lib -lstdc++ --def ..\..\OVModule.def -o OVIMChewing.dll
pause