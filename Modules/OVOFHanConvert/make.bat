del *.*~
cl /I../../Framework/Headers /I..\SharedHeaders /I.\ /EHsc /TP /GR /MT /D"WIN32" /FeOVOFHanConvert.DLL /LD *.cpp *.c ..\SharedSource\*.cpp /link /DEF:..\..\Experiments\Win32\OVModule.def
pause
