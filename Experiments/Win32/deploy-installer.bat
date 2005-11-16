copy .\AmphiVanilla\libltdl3.dll .\Installer\system32\
copy .\AmphiVanilla\libiconv-2.dll .\Installer\system32\
copy .\SQLite3\sqlite3.dll .\Installer\system32\
copy .\tinyxml\Release\tinyxml.dll .\Installer\system32\
copy .\OVIMEUI\Release\OVIMEUI.dll .\Installer\system32\
copy .\OVIME\Release\ovime.ime .\Installer\system32\
copy .\OVPreferences\Release\OVPreferences.exe .\Installer\OpenVanilla\
copy .\OVPreferences\Release\OVPreferences.exe.manifest .\Installer\OpenVanilla\
xcopy /e .\OVPreferences\Release\zh_TW .\Installer\OpenVanilla\zh_TW\
copy .\Modules\OVIMTobacco\Release\OVIMTobacco.dll .\Installer\OpenVanilla\Modules\
copy .\Modules\OVIMRomanNew\Release\OVIMRomanNew.dll .\Installer\OpenVanilla\Modules\
pause
