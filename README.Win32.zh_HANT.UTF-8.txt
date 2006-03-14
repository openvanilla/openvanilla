OpenVanilla 0.7.1 / 0.7.2: Framework, Loaders and Modules 
Copyright (c) 2004-2006 The OpenVanilla Project

== 警告！警告！警告！ ==

自 r1588 開始，OpenVanilla 推進至 0.7.2 ，由於有大量目錄經過了搬動，目前
Windows 的程式碼中，泛用輸入法模組 (Modules/OVIMGeneric) 仍無法編譯。若要測試
完全可用的版本，請改用以下目錄的程式碼：

    http://svn.openfoundry.org/openvanilla/branches/0.7.1

或是改拿 r1588（含）之前的 trunk 原始碼來編譯。

若不需要編譯 OVIMGeneric，則可直接使用 trunk 的最新版，但仍請注意以下事項。

== 注意事項 ==
現在 trunk/Experiments/Win32/ 底下的是 socket-based loader，可能還不適合用在
beta 版上。
請改用 branches/Non-client-server Win32 Loader/ 底下的版本。:)
也就是說，請以 branches/Non-client-server Win32 Loader/ 搭配 trunk/ 的其它目錄
一起進行編譯。
建議的方式是 svn co branches/Non-client-server Win32 Loader/ 到你本機目錄裡的
trunk/Experiments/Win32-nonsocket/，假裝這個目錄是 trunk 的一部分，比較容易處理
編譯時的相對路徑設定。請放心，這不會把 trunk 搞亂，因為 commit 回去時仍會存進
branches/Non-client-server Win32 Loader/。

若遇到任何問題，請透過 IRC，到 FreeNote 的 #im-dev 找 kanru 或 pcman 或 b6s。

== 編譯 ==
Windows 版現在仍在 trunk/Experiments/Win32/ 中，穩定之後才會移進
trunk/Loaders/Win32/。

1. 首先需要安裝 wxWidgets (http://www.wxwidgets.org/dl_msw2.htm#stable)，
裝完之後 *必須* 自行編譯，通常直接用 VC++ 開啟 wxWidgets 附的 .dsw 再編譯即可。

2. 接下來得設定 wxWidgets 標頭檔和函式庫的路徑，請參考：

	http://wiki.wxwidgets.org/wiki.pl?MSVC_.NET_Setup_Guide
	http://wiki.wxwidgets.org/wiki.pl?MSVC_Setup_Guide

如果使用 wxwidgets-2.6.1 又安裝在預設目錄，那麼幾乎可以直接用 OVIMEUI.vcproj 裡
預先寫好的路徑。

3. 如果有 VC.NET 2003 IDE，開啟 trunk/Experments/Win32/OVIME.sln 應能直接編譯。
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
