rem xcopy /s "\Documents and Settings\All Users\Documents\VisualBasic"\*.* .\
del *.*~
cl /I.\OpenVanilla\ /EHsc /FeOVIMPOJ.DLL /LD *.cpp /link /DEF:OVIMPOJ.DEF
