OpenVanilla 0.7.1 / 0.7.2: Framework, Loaders and Modules 
Copyright (c) 2004-2006 The OpenVanilla Project


== 注意事項 ==
1. 目前只能用 Microsoft Visual C++ 編譯。最好使用 VC 7.1 (VS.NET 2003)。

2. 現在 trunk/Experiments/Win32/ 底下的是 socket-based loader，可能還不適合用在
beta 版上。
請改用 branches/Non-client-server Win32 Loader/ 底下的版本。:)
也就是說，請以 branches/Non-client-server Win32 Loader/ 搭配 trunk/ 的其它目錄
一起進行編譯。
建議的方式是 svn co "branches/Non-client-server Win32 Loader/" 到你本機目錄裡的
"trunk/Experiments/Win32-nonsocket/"，假裝這個目錄是 trunk 的一部分，比較容易處
理編譯時的相對路徑設定。請放心，這不會把 trunk 搞亂，因為 commit 回去時仍會存進
"branches/Non-client-server Win32 Loader/"。

3. 若遇到任何問題，請透過 IRC 到 FreeNote 的 #im-dev 找 kanru 或 pcman 或 b6s。


== 編譯 ==
Windows 版現在仍在 trunk/Experiments/Win32/ 中，穩定之後才會移進
trunk/Loaders/Win32/。

1. 首先，為了編譯 OVPreferences，必須安裝 wxWidgets Win32 版 (http://www.wxwidgets.org/dl_msw2.htm#stable)，
裝完之後 *必須* 自行編譯出 OVPreferences 所需的靜態函式庫。目前 wxWidgets 版本
是 2.6.3，為避免經歷與 OV 無關的痛苦，請按以下步驟進行：

1.1. 開啟命令提示字元，然後執行
"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"
每次進入命令列模式進行一連串編譯前，請先確定是否做了這一步。
1.2. 到 C:\wxWidgets-2.6.3\build\msw 底下執行
nmake -f makefile.vc BUILD=release UNICODE=1 SHARED=0 RUNTIME_LIBS=static
和
nmake -f makefile.vc BUILD=debug UNICODE=1 SHARED=0 RUNTIME_LIBS=static

2. 接下來有兩個選擇：用 VS.NET 2003 IDE 編譯，或在命令列環境底下用 nmake 編譯。

2.1. 如果有 VS.NET 2003 IDE，開啟 OVIME.sln 應能直接編譯。接著請開啟
trunk/Experiments/Win32/OVPreferences.sln 以編譯設定選單。欲編譯時，可按需求選
擇 Debug 或 Release 模式，再執行「建置方案」即可。
2.2. 喜歡命令列模式的，請先設定環境變數 WXWIN=C:\wxWidgets-2.6.3\ ，然後在執行
make.bat ，編譯成果會放在 Result 目錄下。

3. 最後，選擇想要使用的模組，到該目錄下開啟 .vcproj 檔編譯，或執行裡面的
make.bat 即可。


== 測試 ==
在此以 OVIMPOJ-Holo 為例
0. 若未曾安裝過 OV，請執行 OVIME.reg。
1. 「按需求修改」deploy-debug.bat 然後執行（不知道怎麼改的話請上 IRC 詢問）。
1.1. 之前用命令列模式編譯者，請「按需求修改」 make_install.bat。
2. 把 poj-holo.cin 放到 %WINDIR%\OpenVanilla\OVIMPOJ-Holo 裡
3. 開啟輸入法設定視窗, 新增剛剛裝入的輸入法
4. 使用 VC.NET 2003 的 debug 模式，開啟 OV，讓 OV attach 到某文字編輯器上即可。

== 製作安裝檔 ==
* 安裝 NSIS (http://nsis.sourceforge.net/download/)
* 安裝 NSIS 的 Registry plugin (http://nsis.sourceforge.net/wiki/Registry_plugin)
* 把編譯好的檔案放在 trunk/Experiments/Win32/Installer/ 底下
** 要安裝到 %WINDIR%\system32 的檔案就放在 trunk/Experiments/Win32/Installer/System32/ 裡
** 要安裝到 %WINDIR%\OpenVanilla 的檔案就放在 trunk/Experiments/Win32/Installer/ 裡
* 在對應的 .nsi 檔上按右鍵選 Compile NSIS Script
