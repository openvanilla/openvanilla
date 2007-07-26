mkdir ".\Installer\system32"
mkdir ".\Installer\OpenVanilla"
mkdir ".\Installer\OpenVanilla\Modules"
mkdir ".\Installer\OpenVanilla\Modules\OVIMGeneric"
mkdir ".\Installer\OpenVanilla\Modules\OVIMRomanNew"
copy "%OV_DEPS%\libltdl3.dll" .\Installer\system32\
copy "%OV_DEPS%\libiconv-2.dll" .\Installer\system32\
copy "%OV_DEPS%\sqlite3.dll" .\Installer\system32\
copy "%OV_DEPS%\tinyxml.dll" .\Installer\system32\
copy "%OV_DEPS%\hunspelldll.dll" .\Installer\system32\
copy ".\CSharpFormLibrary\bin\Release\CSharpFormLibrary.dll" .\Installer\OpenVanilla\
copy "..\..\Utilities\GacUtil\bin\Release\GacUtil.exe" .\Installer\OpenVanilla\
copy .\OVIMEUIManaged\Release\OVIMEUI.dll .\Installer\system32\
copy .\OVIME\Release\OVIME.ime .\Installer\system32\
copy .\OVPreferences\Release\OVPreferences.exe .\Installer\OpenVanilla\
copy .\OVPreferences\Release\OVPreferences.exe.manifest .\Installer\OpenVanilla\
xcopy /e .\OVPreferences\Release\zh_TW .\Installer\OpenVanilla\zh_TW\
REM copy "..\..\..\branches\0.7.2-Module-OVIMTobacco-Old\OVIMTobaccoOld\Release\OVIMTobaccoOld.dll" ".\Installer\OpenVanilla\Modules\OVIMTobacco.dll"
copy "..\..\Modules\OVIMGeneric\Release\OVIMGeneric.dll" ".\Installer\OpenVanilla\Modules\"
copy "..\..\Modules\OVIMRomanNew\Release\OVIMRomanNew.dll" ".\Installer\OpenVanilla\Modules\"
copy "..\..\Modules\OVIMRomanNew\en_US.*" ".\Installer\OpenVanilla\Modules\OVIMRomanNew\"
pause
