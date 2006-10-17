copy "%OV_DEPS%\libltdl3.dll" .\Installer\system32\
copy "%OV_DEPS%\libiconv-2.dll" .\Installer\system32\
copy "%OV_DEPS%\sqlite3.dll" .\Installer\system32\
copy "%OV_DEPS%\tinyxml.dll" .\Installer\system32\
copy .\CSharpFormLibrary\bin\Release\CSharpFormLibrary.dll .\Installer\OpenVanilla\
copy .\CSharpFormLibrary\CSharpFormLibrary.dll.config .\Installer\OpenVanilla\
copy .\OVIMEUIManaged\Release\OVIMEUI.dll .\Installer\system32\
copy .\OVIME\Release\ovime.ime .\Installer\system32\
copy .\OVPreferences\Release\OVPreferences.exe .\Installer\OpenVanilla\
copy .\OVPreferences\Release\OVPreferences.exe.manifest .\Installer\OpenVanilla\
xcopy /e .\OVPreferences\Release\zh_TW .\Installer\OpenVanilla\zh_TW\
copy .\Modules\OVIMTobacco\Release\OVIMTobacco.dll .\Installer\OpenVanilla\Modules\
copy .\Modules\OVIMRomanNew\Release\OVIMRomanNew.dll .\Installer\OpenVanilla\Modules\
pause
