call "c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat"
cd "tinyxml"
call .\make.bat
cd "..\OVIMEUI\PCMan"
call .\make.bat
copy OVIMEUI.* ..\..\OVIME\
cd ..\..\
call nmake
cd "OVPreferences"
call make
cd ..\
copy OVIME\OVIMEUI.DLL Result\
copy OVPreferences\OVPreferences.exe Result\
del Result\*.obj
del Result\*.exp
del Result\*.lib
del Result\*.pdb
del Result\*.res