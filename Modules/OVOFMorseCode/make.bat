del *.*~
cl /I..\..\Headers /I.\ /EHsc /TP /GR /MT /D"WIN32" /FeOVOFMorseCode.DLL /LD *.cpp ..\..\Source\*.cpp /link /DEF:..\..\Experiments\Win32\OVModule.def
pause
