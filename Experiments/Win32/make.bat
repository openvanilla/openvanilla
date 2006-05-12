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
copy OVPreferences\OVPreferences.exe.manifest Result\
copy AmphiVanilla\libltdl3.dll Result\
copy AmphiVanilla\libiconv-2.dll Result\
copy SQLite3\sqlite3.dll Result\
copy tinyxml\Debug\tinyxml.dll Result\
del Result\*.obj
del Result\*.exp
del Result\*.lib
del Result\*.pdb
del Result\*.res