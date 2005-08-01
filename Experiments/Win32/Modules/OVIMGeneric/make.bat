del *.*~
cl /I.\ /I..\..\..\..\Headers /GR /EHsc /D OV_DEBUG /FeOVIMGeneric.DLL /D WIN32 /LD *.cpp ..\..\..\..\Source\*.cpp /link /DEF:..\..\OVModule.DEF
pause