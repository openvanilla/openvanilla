del *.*~
cl /O1 /I.\ /I..\..\..\..\Headers /I..\ /I..\..\OVDummyLoader /GR /EHsc /MT /FeOVDCPCMan.DLL /D WIN32 /LD *.cpp ..\Utility.cpp ..\..\..\..\Source\*.cpp ..\..\OVDummyLoader\OVDisplayComponent.cpp /link /DEF:..\..\OVModule.DEF User32.lib gdi32.lib
pause