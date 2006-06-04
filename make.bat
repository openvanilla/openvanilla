call "c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat"
cd "tinyxml"
call .\make.bat
cd "..\CSharpFormLibrary"
csc /out:CSharpFormLibrary.dll /t:library *.cs
cd "..\OVIMEUI\PCMan"
call .\make.bat
copy OVIMEUI.* ..\..\OVIME\
cd ..\..\
call nmake
cd "OVPreferences"
call make
cd ..\
copy OVIME\OVIMEUI.DLL Result\
copy CSharpFormLibrary\CSharpFormLibrary.dll Result\
copy OVPreferences\OVPreferences.exe Result\
copy OVPreferences\OVPreferences.exe.manifest Result\
copy AmphiVanilla\libltdl3.dll Result\
copy AmphiVanilla\libiconv-2.dll Result\
copy tinyxml\tinyxml.dll Result\
del Result\*.obj
del Result\*.exp
del Result\*.lib
del Result\*.pdb
del Result\*.res