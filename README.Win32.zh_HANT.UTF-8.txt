OpenVanilla 0.7.1 / 0.7.2: Framework, Loaders and Modules
Copyright (c) 2004-2006 The OpenVanilla Project


== 注意事項 ==
1. 目前只能用 Microsoft Visual C++ 編譯。最好使用 VC 7.1 (VS.NET 2003)。

2. 請使用 trunk/Loaders/Win32/ 底下的 OVIME.sln 和 OVPreferences.sln 及
trunk/Modules/Modules.sln 來編譯。

3. 原先在 repository 裡的 SQLite3, ltdl, iconv, tinyxml 皆已移除，請下載
http://taipedia.info/OV-deps.zip ，解壓縮後置於某處，再按照
http://www.flickr.com/photos/b6s/159926134/ 示範的方式讓 VS.NET 2003 知道要去那
裡找這些函式庫。
3.1. 為了之後布署方便，請設定環境變數 %OV_DEPS% 到這些函式庫所在的目錄，
include files 和 library files 的 path 都要記得加上

4. 若遇到任何問題，請透過 IRC 到 FreeNote 的 #im-dev 找 kanru 或 pcman 或 b6s。


== 編譯 ==
1. 首先，為了編譯 OVPreferences，必須安裝 wxWidgets Win32 版
(http://www.wxwidgets.org/dl_msw2.htm#stable)，裝完之後 *必須* 自行編譯出
OVPreferences 所需的靜態函式庫。目前 wxWidgets 版本
是 2.6.3，為避免經歷與 OV 無關的痛苦，請按以下步驟進行：

1.1. 開啟命令提示字元，然後執行
"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"
每次進入命令列模式進行一連串編譯前，請先確定是否做了這一步。
1.2. 到 C:\wxWidgets-2.6.3\build\msw 底下執行
nmake -f makefile.vc BUILD=release UNICODE=1 SHARED=0 RUNTIME_LIBS=static
和
nmake -f makefile.vc BUILD=debug UNICODE=1 SHARED=0 RUNTIME_LIBS=static

2. 如果有 VS.NET 2003 IDE，開啟 OVIME.sln 應能直接編譯。

3. 接著請開啟 OVPreferences.sln 以編譯設定選單。

4. 最後，開啟 Modules.sln 以編譯目前支援的模組。


== 測試 ==
在此以 OVIMPOJ-Holo 為例

1. 按照上一節所述，完成 OVIME.sln 與 OVPreferences.sln 的編譯，然後在
Modules.sln 裡編譯 OVIMPOJ-Holo 模組。

2. 手動建立 %WINDIR%\OpenVanilla 及 %WINDIR%\OpenVanilla\Modules

3. 布署檔案，請參考 trunk/Loaders/Win32/deploy-debug.bat，或直接使用之。
3.1. 把「注意事項 3」提到的 DLLs 放進 %SYSTEM32% (%WINDIR%\system32)
3.2. 把 OVIME.sln 編譯出來的 OVIME.ime 及 OVIMEUI.dll 放進 %SYSTEM32%
3.3. 把 OVPreferences.sln 編譯出來的 OVPreferences.exe 放進 %WINDIR%\OpenVanilla
3.4. 把 OVIMPOJ-Holo.dll 放進 %WINDIR%\OpenVanilla\Modules

4. 把 poj-holo.cin 放到 %WINDIR%\OpenVanilla\OVIMPOJ-Holo 裡；當然，這個目錄必
須先手動建立。

5. 若未曾安裝過 OV，請執行 OVIME.reg。

6. 開啟輸入法設定視窗, 新增剛剛裝入的輸入法

7. 使用 VC.NET 2003 的 debug 模式，開啟 OV，依照下列網址中的圖解操作：
http://openvanilla.org/wiki/zh/index.php?title=%E8%88%8Awiki%E9%A0%81%E9%9D%A2%E5%BD%99%E6%95%B4:Win32%E7%AD%86%E8%A8%98#Debugging_Instructions


== 製作安裝檔 ==
* 安裝 NSIS (http://nsis.sourceforge.net/download/)
* 安裝 NSIS 的 Registry plugin (http://nsis.sourceforge.net/wiki/Registry_plugin)
* 把編譯好的檔案放在 trunk/Experiments/Win32/Installer/ 底下
** 要安裝到 %WINDIR%\system32 的檔案就放在 trunk/Experiments/Win32/Installer/System32/ 裡
** 要安裝到 %WINDIR%\OpenVanilla 的檔案就放在 trunk/Experiments/Win32/Installer/ 裡
* 在對應的 .nsi 檔上按右鍵選 Compile NSIS Script
