del *.*~
cl /I.\Headers /EHsc  /GR /FeOVDummyLoader.DLL /LD *.cpp .\Source\*.cpp /link .\libltdl.lib /DEF:OVDummyLoader.DEF /DEBUG