call .\deploy-dependencies.bat
copy .\CSharpFormLibrary\bin\Release\CSharpFormLibrary.dll %WINDIR%\OpenVanilla\
copy .\OVIMEUIManaged\Release\OVIMEUI.dll %WINDIR%\system32\
copy .\OVIME\Release\ovime.ime %WINDIR%\system32\
copy .\OVPreferences\Release\OVPreferences.exe %WINDIR%\OpenVanilla\
copy .\OVPreferences\Release\OVPreferences.exe.manifest %WINDIR%\OpenVanilla\
copy ..\..\Modules\OVIMGeneric\Release\OVIMGeneric.dll %WINDIR%\OpenVanilla\Modules\
pause
