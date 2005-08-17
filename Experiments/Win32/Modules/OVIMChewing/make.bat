del *.*~
cl /I.\ /I..\..\..\..\Headers /GR /EHsc /MT /FeOVIMChewing.DLL /D WIN32 /LD *.cpp .\ChewingPP\Chewing.cpp ..\..\..\..\Source\*.cpp /link .\libchewing.lib /DEF:..\..\OVModule.DEF
pause