OpenVanilla 0.7.1 / 0.7.2: Framework, Loaders and Modules
Copyright (c) 2004-2006 The OpenVanilla Project


== 注意事項 ==
1. 目前只能用 Microsoft Visual C++ 8 (VS.NET 2005) 編譯。

2. 請使用 trunk/Loaders/Win32/ 底下的 OVIME.sln 和 trunk/Modules/Modules.sln
來編譯。

3. 原先在 repository 裡的 ltdl, iconv, tinyxml 皆已移除，請下載
http://taipedia.selfip.info/OV-deps.zip 「或」
http://rt.openfoundry.org/Foundry/Project/Download/Attachment/89705/61781/OV-deps.zip
（兩個是一樣的），解壓縮後置於某處－－例如 "your_dependency_path"－－再按照

http://www.flickr.com/photos/b6s/159926134/ 示範的方式讓 VS.NET 2005 知道要去那
裡找這些函式庫。
3.1. 為了之後布署方便，請設定環境變數 %OV_DEPS% 到 "your_dependency_path"，
include files 和 library files 的 path 都要記得加上。
3.2. 若想在 Vista 上用 IE7, 請「不要」看底下的 3.2.1.。上述 OV-deps.zip 裡的
sqlite3.dll 是 http://sqlite.org/sqlitedll-3_2_5.zip. 這是最近發現的問題，詳細
狀況請參考 http://article.gmane.org/gmane.comp.db.sqlite.general/29356 的說明。
3.2.1. SQLite3 也已自 svn repository 移除，若想用「比 OV-deps.zip 還新的」，請到
http://taipedia.selfip.info/mediawiki/index.php/SQLite3Win32LibVC8/zh-hant
下載，並同樣放在 %OV_DEPS% 裡。

4. OV-deps 已加入 OVIMRomanNew 所需要的拼字檢查 library -- hunspell, 檔案如下：
affentry.hxx, affixmgr.hxx, atypes.hxx, baseaffix.hxx, csutil.hxx, dictmgr.hxx,
hashmgr.hxx, htypes.hxx, hunspell.h, hunspell.hxx, hunspelldll.dll, hunspelldll.h,
hunspelldll.lib, license.hunspell, license.myspell, langnum.hxx, suggestmgr.hxx
4.1. 比照 3. 的說明，請替 VS.NET 2005 的 include directory 和 library directory
加上 "your_dependency_path"\hunspell



5. 若遇到任何問題，請透過 IRC 到 FreeNote 的 #im-dev 找 b6s。


== 編譯 ==
1. 如果有 VS.NET 2005 IDE，開啟 OVIME.sln 應能直接編譯。

1. 首先，為了編譯 OVPreferences，必須安裝 wxWidgets Win32 版
(http://www.wxwidgets.org/dl_msw2.htm#stable)，裝完之後「必須」自行編譯出
OVPreferences 所需的靜態函式庫。目前 wxWidgets 版本
是 2.6.3，為避免經歷與 OV 無關的痛苦，請按以下步驟進行：


3. 最後，開啟 Modules.sln 以編譯目前支援的模組。


== 測試 ==
在此以 OVIMPOJ-Holo 為例

1. 按照上一節所述，完成 OVIME.sln 與 OVPreference.CS2.sln 的編譯，然後在
Modules.sln 裡編譯 OVIMPOJ-Holo 模組。

2. 手動建立 %WINDIR%\OpenVanilla 及 %WINDIR%\OpenVanilla\Modules

3. 布署檔案，請參考 trunk/Loaders/Win32/deploy-debug.bat，或直接使用之。
3.1. 把「注意事項 3」提到的 DLLs 放進 %SYSTEM32% (%WINDIR%\system32)

4. 把 poj-holo.cin 放到 %WINDIR%\OpenVanilla\OVIMPOJ-Holo 裡；當然，這個目錄必
須先手動建立。

5. 若未曾安裝過 OV，請執行 OVIME.reg。

6. 開啟輸入法設定視窗, 新增剛剛裝入的輸入法

7. 使用 VC.NET 2005 的 debug 模式，開啟 OV，依照下列網址中的圖解操作：
http://openvanilla.org/wiki/zh/index.php?title=%E8%88%8Awiki%E9%A0%81%E9%9D%A2%E5%BD%99%E6%95%B4:Win32%E7%AD%86%E8%A8%98#Debugging_Instructions


== 製作安裝檔 ==
* 安裝 NSIS (http://nsis.sourceforge.net/download/)
* 安裝 NSIS 的 Registry plugin (http://nsis.sourceforge.net/wiki/Registry_plugin)
* 把編譯好的檔案放在 trunk/Experiments/Win32/Installer/ 底下
** 要安裝到 %WINDIR%\system32 的檔案就放在 trunk/Loaders/Win32/Installer/System32/ 裡
** 要安裝到 %WINDIR%\OpenVanilla 的檔案就放在 trunk/Loaders/Win32/Installer/ 裡
* 在對應的 .nsi 檔上按右鍵選 Compile NSIS Script
