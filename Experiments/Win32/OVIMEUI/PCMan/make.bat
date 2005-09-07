del *.*~
rc /fo PCMan.res PCMan.rc
cl /I.\ /I..\Headers /EHsc /GR /MT /D "_UNICODE" /D "UNICODE" /FeOVIMEUI.DLL /LD *.cpp ..\Source\*.cpp /link PCMan.res /DEF:..\OVIMEUI.DEF /SUBSYSTEM:WINDOWS kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib imm32.lib