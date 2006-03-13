cl /I.\ /I..\..\..\..\Framework\Headers /I..\..\..\..\Modules\SharedHeaders /GR /EHsc /MT /FeOVIMArray.DLL /D WIN32 /LD *.cpp ..\..\..\..\Modules\SharedSource\*.cpp /link /DEF:..\..\OVModule.DEF
pause
