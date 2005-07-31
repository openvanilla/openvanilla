del *.*~
cl /I..\..\Headers /I.\sqlite-src /GR /EHsc /FeOVIMGenericSQLite.DLL /LD OVIMGenericSQLite.cpp ./sqlite-src/*.c /link /DEF:..\..\Experiments\Win32\OVModule.DEF
pause