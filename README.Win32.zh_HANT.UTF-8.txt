OpenVanilla 0.7.1 / 0.7.2: Framework, Loaders and Modules
Copyright (c) 2004-2006 The OpenVanilla Project


== 注意事項 ==
1. 目前只能用 Microsoft Visual C++ 8 (VS.NET 2005) 編譯。

2. 請使用 trunk/Loaders/Win32/ 底下的 OVIME.sln 和 trunk/Modules/Modules.sln
來編譯。

3. 原先在 repository 裡的 ltdl, iconv, tinyxml 皆已移除，請下載
http://openvanilla.googlecode.com/files/OV-deps@200710241800.zip，解壓縮後置於
某處 －－ 例如 "your_dependency_path" －－ 再按照
http://www.flickr.com/photos/b6s/159926134/ 示範的方式讓 VS.NET 2005 知道要去那
裡找這些函式庫 (Library files) 及標頭檔 (Include files)；圖示僅為 Include files,
Library files 也請照做一遍。
3.1. 為了之後布署方便，請設定環境變數 %OV_DEPS% 到 "your_dependency_path"，同樣
地，Include files 和 Library files 的 path 都要記得加上。

4. OV-deps 已加入 OVIMRomanNew 所需要的拼字檢查 library -- hunspell, 18 個檔案如下：
affentry.hxx, affixmgr.hxx, atypes.hxx, baseaffix.hxx, csutil.hxx, dictmgr.hxx, hashmgr.hxx,
htypes.hxx, hunspell.h, hunspell.hxx, libhunspell.dll, libhunspell.lib, hunspelldll.h, license.hunspell,
license.myspell, langnum.hxx, phonet.hxx, suggestmgr.hxx
4.1. 比照 3. 的說明，請替 VS.NET 2005 的 Include directory 和 Library directory
加上 "your_dependency_path"\hunspell
4.2. 您也可由 hunspell 原始碼自行編譯出 libhunspell.dll 及 libhunspell.lib, 以
便自行與 hunspell 最新版同步。用 VS.NET 2005 開啟
    %your_hunspell_src_dir%/src/win_api/Hunspell.sln
在 Debug_dll 或 Release_dll 配置下編譯即可。
4.3. 請下載 http://ftp.services.openoffice.org/pub/OpenOffice.org/contrib/dictionaries/en_US.zip
並解壓縮，將其中的 en_US.dic 和 en_US.aff 置於 Modules/OVIMRomanNew/ 底下。

5. 若遇到任何問題，請透過 IRC 到 FreeNote 的 #im-dev 找 b6s。


== 編譯 ==
1. 如果有 VS.NET 2005 IDE，開啟 OVIME.sln 應能直接編譯。

2. 接著請開啟 OVPreference.CS2.sln 以編譯設定選單。

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
