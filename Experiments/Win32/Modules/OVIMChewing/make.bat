del *.*~
cl /I.\ /I..\..\..\..\Headers /GR /EHsc /FeOVIMChewing.DLL /D WIN32 /LD *.cpp .\ChewingPP\Chewing.cpp ..\..\..\..\Source\*.cpp /link .\libchewing.lib /DEF:..\..\OVModule.DEF
pause