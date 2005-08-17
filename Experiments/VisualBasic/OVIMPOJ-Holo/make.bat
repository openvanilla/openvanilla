cl /I..\..\..\Headers /I.\ /I..\ /EHsc /GR /MT /D"WIN32" /FeOVIMPOJ-Holo.DLL /LD ..\OVIMPOJ-Holo.cpp ..\POJ-Holo.cpp ..\..\..\Source\*.cpp /link /DEF:..\..\Win32\OVModule.def
pause