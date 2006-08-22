call "c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat"
REM cd "tinyxml"
REM call .\make.bat
REM cd "..\CSharpFormLibrary"
REM csc /out:CSharpFormLibrary.dll /t:library *.cs
REM cd "..\OVIMEUIManaged\PCMan"
cd "OVIMEUIManaged\PCMan"
call .\make.bat
copy OVIMEUI.* ..\..\OVIME\
cd ..\..\
call nmake
cd "OVPreferences"
call .\make.bat
cd ..\
copy OVIME\OVIMEUI.DLL Result\
REM copy CSharpFormLibrary\CSharpFormLibrary.dll Result\
copy OVPreferences\OVPreferences.exe Result\
copy OVPreferences\OVPreferences.exe.manifest Result\
copy %OV_DEPS%\libltdl3.dll Result\
copy %OV_DEPS%\libiconv-2.dll Result\
copy %OV_DEPS%\tinyxml.dll Result\
del Result\*.obj
del Result\*.exp
del Result\*.lib
del Result\*.pdb
del Result\*.res