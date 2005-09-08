del *.*~
cl /I.\ /I..\..\..\Headers -I..\tinyxml /GR /EHsc /MT /D OV_DEBUG /D WIN32 *.cpp ..\..\..\Source\*.cpp /link .\libltdl.lib .\libiconv.lib ..\tinyxml\tinyxml.lib /out:Test.exe
