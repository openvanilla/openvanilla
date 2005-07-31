del *.*~
cl /I..\..\..\Headers /EHsc  /GR /FeOVDummyLoader.DLL /LD *.cpp ..\..\..\Source\*.cpp /link .\libltdl.lib /DEF:OVDummyLoader.DEF /DEBUG
copy OVDummyLoader.DLL ..\Ime\trunk\
copy OVDummyLoader.lib ..\Ime\trunk\
pause