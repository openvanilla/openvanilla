mkdir ".\Installer\system32"
mkdir ".\Installer\OpenVanilla"
mkdir ".\Installer\OpenVanilla\Modules"
mkdir ".\Installer\OpenVanilla\Modules\OVIMGeneric"
mkdir ".\Installer\OpenVanilla\Resources"
copy "%OV_DEPS%\libltdl3.dll" .\Installer\system32\
copy "%OV_DEPS%\libiconv-2.dll" .\Installer\system32\
copy "%OV_DEPS%\sqlite3.dll" .\Installer\system32\
copy "%OV_DEPS%\tinyxml.dll" .\Installer\system32\
copy "%OV_DEPS%\hunspell\hunspell.dll" .\Installer\system32\
copy ".\CSharpFormLibrary\bin\Release\CSharpFormLibrary.dll" .\Installer\OpenVanilla\
copy "..\..\Utilities\GacUtil\bin\Release\GacUtil.exe" .\Installer\OpenVanilla\
copy .\OVIMEUIManaged\Release\OVIMEUI.dll .\Installer\system32\
copy .\OVIME\Release\OVIME.ime .\Installer\system32\
copy .\OVPreferences\bin\Release\OVPreferences.exe .\Installer\OpenVanilla\
copy .\OVPreferences\bin\Release\Resources\*.resources .\Installer\OpenVanilla\Resources
REM copy "..\..\..\branches\0.7.2-Module-OVIMTobacco-Old\OVIMTobaccoOld\Release\OVIMTobaccoOld.dll" ".\Installer\OpenVanilla\Modules\OVIMTobacco.dll"
copy "..\..\Modules\OVIMGeneric\Release\OVIMGeneric.dll" ".\Installer\OpenVanilla\Modules\"
copy "..\..\Modules\OVIMRomanNew\Release\OVIMRomanNew.dll" ".\Installer\OpenVanilla\Modules\"
cd "..\..\Modules\OVIMRomanNew"
call "deploy-hunspell.bat"
pause
