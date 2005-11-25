== 注意事項 ==
現在 trunk/Experiments/Win32/ 底下的是 socket-based loader，可能還不適合用在
beta production 上。
請改用 branches/ 底下的版本。:)

遇到任何問題，請透過 IRC，到 FreeNote 的 #im-dev 找 kanru 或 pcman 或 b6s。

== 編譯 ==
Windows 版現在仍在 trunk/Experiments/Win32/ 中，未來會盡快移出 Experiemtns。

首先需要安裝 wxWidgets (http://www.wxwidgets.org/dl_msw2.htm#stable)，
裝完之後 *必須* 自行編譯，通常直接用 VC++ 開啟 wxWidgets 附的 .dsw 再編譯即可。

接下來得設定 wxWidgets 標頭檔和函式庫的路徑，請參考：

	http://wiki.wxwidgets.org/wiki.pl?MSVC_.NET_Setup_Guide
	http://wiki.wxwidgets.org/wiki.pl?MSVC_Setup_Guide

如果使用 wxwidgets-2.6.1 又安裝在預設目錄，那麼幾乎可以直接用 OVIMEUI.vcproj 裡
預先寫好的路徑。

如果有 VC.NET 2003 IDE，開啟 trunk/Experments/Win32/OVIME.sln，應該能直接編譯。
另外，也請開啟 trunk/Experiments/Win32/OVPreferences.sln 以編譯設定選單。
如果只裝了 VC++ ToolKit (http://msdn.microsoft.com/visualc/vctoolkit2003/)，則
必須在 trunk/Experiments/Win32 底下，按照順序進行：

tinyxml\make.bat
AmphiVanilla\make_server.bat
Win32OVIMEUI\PCMan\make.bat
nmake
OVPreferences\make.bat

== 製作安裝檔 ==
* 安裝 NSIS (http://nsis.sourceforge.net/download/)
* 安裝 NSIS 的 Registry plugin (http://nsis.sourceforge.net/wiki/Registry_plugin)
* 把編譯好的檔案放在 trunk/Experiments/Win32/Installer/ 底下
** 要安裝到 %WINDIR%\system32 的檔案就放在 trunk/Experiments/Win32/Installer/System32/ 裡
** 要安裝到 %WINDIR%\OpenVanilla 的檔案就放在 trunk/Experiments/Win32/Installer/ 裡
* 在對應的 .nsi 檔上按右鍵選 Compile NSIS Script
