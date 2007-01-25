call .\deploy-dependencies.bat
REM copy .\CSharpFormLibrary\bin\Debug\CSharpFormLibrary.dll %WINDIR%\OpenVanilla\
copy .\OVIMEUIManaged\Debug\OVIMEUI.dll %WINDIR%\system32\
copy .\OVIME\Debug\OVIME.ime %WINDIR%\system32\
REM copy .\OVPreferences\Debug\OVPreferences.exe %WINDIR%\OpenVanilla\
REM copy .\OVPreferences\Release\OVPreferences.exe.manifest %WINDIR%\OpenVanilla\
REM copy ..\..\Modules\OVIMGeneric\Debug\OVIMGeneric.dll %WINDIR%\OpenVanilla\Modules\
copy "..\..\..\branches\0.7.2-Module-OVIMTobacco-Old\OVIMTobaccoOld\Debug\OVIMTobaccoOld.dll" "%WINDIR%\OpenVanilla\Modules\OVIMTobacco.dll"
REM del "%APPDATA%\OpenVanilla\orz.txt"
pause
