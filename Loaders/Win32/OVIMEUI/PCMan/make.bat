del *.*~
rc /fo PCMan.res PCMan.rc
cl /O1 /I.\ /I..\Headers /I..\..\AmphiVanilla /I..\..\..\..\Framework\Headers /I..\..\tinyxml /EHsc /GR /MT /D "_UNICODE" /D "UNICODE" /DWIN32 /clr /FeOVIMEUI.DLL /LD *.cpp ..\Source\*.cpp ..\..\AmphiVanilla\AVConfig.cpp ..\..\AmphiVanilla\AVDictionary.cpp /link PCMan.res /DEF:..\OVIMEUI.DEF /SUBSYSTEM:WINDOWS kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib imm32.lib ..\..\tinyxml\tinyxml.lib
