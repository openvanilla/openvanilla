del *.*~
cl /I.\ /I..\..\..\Framework\Headers -I..\tinyxml /GR /EHsc /MT /D OV_DEBUG /D WIN32 *.cpp ..\..\..\Modules\SharedSource\*.cpp /link SHFolder.lib  .\libltdl.lib .\libiconv.lib ..\tinyxml\tinyxml.lib /out:Test.exe
