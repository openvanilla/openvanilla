del *.*~
cl /I.\ /I..\..\..\Headers -I.\tinyxml /GR /EHsc /MT /D OV_DEBUG /D WIN32 *.cpp ..\..\..\Source\*.cpp tinyxml\*.cpp /link .\libltdl.lib .\libiconv.lib /out:Test.exe