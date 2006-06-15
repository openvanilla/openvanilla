call .\deploy-dependencies.bat
copy .\OVIMEUI\Debug\OVIMEUI.dll %WINDIR%\system32\
copy .\OVIME\Debug\ovime.ime %WINDIR%\system32\
copy .\OVPreferences\Debug\OVPreferences.exe %WINDIR%\OpenVanilla\
copy .\OVPreferences\Release\OVPreferences.exe.manifest %WINDIR%\OpenVanilla\
copy ..\..\Modules\OVIMTobacco\Debug\OVIMTobacco.dll %WINDIR%\OpenVanilla\Modules\
pause
