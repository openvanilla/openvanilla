cl /I..\..\Framework\Headers /I..\SharedHeaders /I.\ /EHsc /GR /MT /D"WIN32" /FeOVIMPOJ-Holo.DLL /LD OVIMPOJ-Holo.cpp POJ-Holo.cpp ..\SharedSource\*.cpp /link /DEF:..\..\Experiments\Win32\OVModule.def
pause
