call .\deploy-dependencies.bat
REM copy .\CSharpFormLibrary\bin\Release\CSharpFormLibrary.dll %WINDIR%\OpenVanilla\
copy .\OVIMEUIManaged\Release\OVIMEUI.dll %WINDIR%\system32\
copy .\OVIME\Release\OVIME.ime %WINDIR%\system32\
REM copy .\OVPreferences\Release\OVPreferences.exe %WINDIR%\OpenVanilla\
REM copy .\OVPreferences\Release\OVPreferences.exe.manifest %WINDIR%\OpenVanilla\
REM copy ..\..\Modules\OVIMGeneric\Release\OVIMGeneric.dll %WINDIR%\OpenVanilla\Modules\
copy "..\..\..\branches\0.7.2-Module-OVIMTobacco-Old\OVIMTobaccoOld\Release\OVIMTobaccoOld.dll" "%WINDIR%\OpenVanilla\Modules\OVIMTobacco.dll"
pause
