del *.*~
cl /I..\..\Headers /I.\ /EHsc /TP /GR /MT /D"WIN32" /FeOVOFHanConvert.DLL /LD *.cpp *.c ..\..\Source\*.cpp /link /DEF:..\..\Experiments\Win32\OVModule.def
pause
