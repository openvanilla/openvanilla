call .\deploy-dependencies.bat
copy .\CSharpFormLibrary\bin\Debug\CSharpFormLibrary.dll %WINDIR%\OpenVanilla\
copy .\OVIMEUIManaged\Debug\OVIMEUI.dll %WINDIR%\system32\
copy .\OVIME\Debug\ovime.ime %WINDIR%\system32\
copy .\OVPreferences\Debug\OVPreferences.exe %WINDIR%\OpenVanilla\
copy .\OVPreferences\Release\OVPreferences.exe.manifest %WINDIR%\OpenVanilla\
copy ..\..\Modules\OVIMGeneric\Debug\OVIMGeneric.dll %WINDIR%\OpenVanilla\Modules\
pause
