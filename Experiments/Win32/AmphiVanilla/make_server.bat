del *.*~
cl /I.\ /I..\..\..\Headers -I..\tinyxml /GR /EHsc /MT /D OV_DEBUG /D WIN32 *.cpp ..\..\..\Source\*.cpp /link Ws2_32.lib SHFolder.lib  .\libltdl.lib .\libiconv.lib ..\tinyxml\tinyxml.lib /out:OVServer.exe /SUBSYSTEM:windows /entry:mainCRTStartup
