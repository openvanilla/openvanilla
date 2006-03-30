call "c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat"
cd "C:\Documents and Settings\tmjiang.BARABBAS\My Documents\Visual Studio Projects\OpenVanilla\trunk\Experiments\Win32-nonsocket\OVIMEUI\PCMan"
call .\make.bat
copy OVIMEUI.* ..\..\OVIME\
cd ..\..\
call nmake