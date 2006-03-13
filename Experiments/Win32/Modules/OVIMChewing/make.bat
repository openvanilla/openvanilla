del *.*~
cl /I.\ /I..\..\..\..\Framework\Headers /I..\..\..\..\Modules\SharedHeaders /GR /EHsc /MT /FeOVIMChewing.DLL /D WIN32 /LD *.cpp .\ChewingPP\Chewing.cpp ..\..\..\..\Modules\SharedSource\*.cpp /link .\libchewing.lib /DEF:..\..\OVModule.DEF
pause