del *.*~
cl /I..\..\Headers /I..\..\Experiments\Tobacco /I.\sqlite-src /GR /EHsc /FeOVIMTobacco.DLL /LD OVIMTobacco.cpp ..\..\Experiments\Tobacco\*.cpp .\sqlite-src\*.c /link /DEF:..\..\Experiments\Win32\OVModule.DEF
pause