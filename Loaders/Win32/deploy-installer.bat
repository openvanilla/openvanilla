mkdir ".\Installer\system32"
mkdir ".\Installer\OpenVanilla"
mkdir ".\Installer\OpenVanilla\Modules"
mkdir ".\Installer\OpenVanilla\Modules\OVIMArray"
mkdir ".\Installer\OpenVanilla\Modules\OVIMGeneric"
mkdir ".\Installer\OpenVanilla\Modules\OVIMRomanNew"
mkdir ".\Installer\OpenVanilla\Modules\OVIMTobacco"
mkdir ".\Installer\OpenVanilla\Resources"
copy "%OV_DEPS%\libltdl3.dll" .\Installer\system32\
copy "%OV_DEPS%\sqlite3.dll" .\Installer\system32\
copy "%OV_DEPS%\hunspell\hunspell.dll" .\Installer\system32\
copy ".\CSharpFormLibrary\bin\Release\CSharpFormLibrary.dll" .\Installer\OpenVanilla\
copy "..\..\Utilities\GacUtil\bin\Release\GacUtil.exe" .\Installer\OpenVanilla\
copy .\OVIMEUIManaged\Release\OVIMEUI.dll .\Installer\system32\
copy .\OVIME\Release\OVIME.ime .\Installer\system32\
copy .\OVPreferences\bin\Release\OVPreferences.exe .\Installer\OpenVanilla\
copy .\OVPreferences\bin\Release\Resources\*.resources .\Installer\OpenVanilla\Resources
copy "..\..\Modules\OVIMArray\Release\OVIMArray.dll" ".\Installer\OpenVanilla\Modules\"
copy "..\..\Modules\SharedData\array*.cin" ".\Installer\OpenVanilla\Modules\OVIMArray\"
copy "..\..\Modules\OVIMGeneric\Release\OVIMGeneric.dll" ".\Installer\OpenVanilla\Modules\"
copy "..\..\Modules\SharedData\dayi3-patched.cin" ".\Installer\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\Modules\SharedData\ehq-symbols.cin" ".\Installer\OpenVanilla\Modules\OVIMGeneric\"
copy "..\..\Modules\OVIMRomanNew\Release\OVIMRomanNew.dll" ".\Installer\OpenVanilla\Modules\"
copy "..\..\Modules\OVIMRomanNew\en_US.aff" ".\Installer\OpenVanilla\Modules\OVIMRomanNew\"
copy "..\..\Modules\OVIMRomanNew\en_US.dic" ".\Installer\OpenVanilla\Modules\OVIMRomanNew\"
copy "..\..\Modules\OVIMTobacco\Release\OVIMTobacco.dll" ".\Installer\OpenVanilla\Modules\"
copy "..\..\Modules\OVIMTobacco\imtables.db" ".\Installer\OpenVanilla\Modules\OVIMTobacco\"
copy "..\..\Modules\OVIMTobacco\tsi.db" ".\Installer\OpenVanilla\Modules\OVIMTobacco\"
pause
