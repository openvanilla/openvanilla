call .\deploy-dependencies.bat
copy .\CSharpFormLibrary\bin\Debug\CSharpFormLibrary.dll %WINDIR%\OpenVanilla\
copy .\CSharpFormLibrary\CSharpFormLibrary.config %WINDIR%\OpenVanilla\
copy .\OVIMEUIManaged\Debug\OVIMEUI.dll %WINDIR%\system32\
copy .\OVIME\Debug\ovime.ime %WINDIR%\system32\
copy .\OVPreferences\Debug\OVPreferences.exe %WINDIR%\OpenVanilla\
copy .\OVPreferences\Release\OVPreferences.exe.manifest %WINDIR%\OpenVanilla\
REM copy ..\..\Modules\OVIMGeneric\Debug\OVIMGeneric.dll %WINDIR%\OpenVanilla\Modules\
copy "..\..\..\branches\0.7.2-Module-OVIMTobacco-Old\OVIMTobaccoOld\Debug\OVIMTobaccoOld.dll" "%WINDIR%\OpenVanilla\Modules\OVIMTobacco.dll"
del C:\WINDOWS\OpenVanilla\orz.txt
pause
