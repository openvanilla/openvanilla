OpenVanilla 0.9.0a1 for Mac OS X 10.4/10.5/10.6
===============================================

Hi all,

我們剛剛發佈了 Mac 上的 OpenVanilla 0.9.0 alpha 1 。下載網址如下：

OS X Leopard/Snow Leopard 版本：

    http://cloud.github.com/downloads/lukhnos/openvanilla-oranje/OpenVanilla-OSX-Leopard-0.9.0a1.zip
    
OS X Tiger 版本：

    http://cloud.github.com/downloads/lukhnos/openvanilla-oranje/OpenVanilla-OSX-Tiger-0.9.0a1.zip

這個版本使用 OV 0.9 "oranje" 分支的程式碼，這個分支是從 OV 主幹分出去的計畫，程式碼放在 github 上：

    http://github.com/lukhnos/openvanilla-oranje

要注意的幾個地方：

1.  這個版本需要手動安裝，請參考 zip 檔裡的 HowToInstall.txt。解除安裝方法亦同。

2.  限於時間，我們只做了少量的安裝測試，分別是：

    *   Mac OS X 10.4 on PowerPC Mac
    *   Mac OS X 10.5 on Intel Mac
    *   Mac OS X 10.5 on Intel Mac
    *   Mac OS X 10.6 on Intel Mac (32-bit)
    
    應用程式的部份僅測試了 TextEdit.app。
    
    請注意：輸入法屬於重要系統元件，系統可能因為輸入法軟體的bug而造成不穩或循環crash。如果發生這種情況，請以其他使用者登入後，砍掉輸入法元件檔案。
 
3.  沒有中文localization

4.  選字窗的形式不能更改

5.  沒有針對特定輸入法模組設定快捷鍵的功能

6.  沒有快捷鍵切換 output filter 的功能，output filter 不能調整處理順序

7.  Loader 會載入所有模組，沒有選用載入功能

8.  需要使用 OVIMSpaceChewing (酷音) 模組的使用者，請自行從以下網址下載：

        http://openvanilla.googlecode.com/files/OVIMSpaceChewingBundle-for-OV-Mac-0.9.0a1.zip
   
    這是 OV 0.8 所使用的 OVIMSpaceChewing module。下載完後，請將模組安裝在：
   
        /Library/OpenVanilla/0.9-Old/Modules/
   
    如果沒有這個目錄，請自行建立。請注意！這個酷音模組是 2007 年年底的版本 (http://svn.openvanilla.org/libchewing) ，跟目前的主幹可能已經有不少差距。當時的版本對資料檔的path總長度有限制，因此必須放在以上位置。
    
    OVIMSpaceChewing 是利用 libchewing 所建立的 OV 輸入法模組，原始程式碼放在：
    
        https://openvanilla.googlecode.com/svn/trunk/Modules/OVIMSpaceChewing
    
    在此也徵求對 libchewing 及 OS X 開發熟悉的朋友，將 zonble 跟我在 2007 年所 fork 出的上述 libchewing code 重新接上當前 libchewing 的進度，製作出最新版本的 OVIMSpaceChewing.bundle。
   
9.  偏好設定程式同樣是重寫過的。需要手動更改設定的話，設定檔放在：

        ~/Library/Preferences/org.openvanilla.plist
   
10.  泛用模組 (OVIMGeneric) 的使用者自定表格，位置在：

        ~/Library/Application Support/OpenVanilla/UserData/OVIMGeneric

11.  這個版本使用跟 0.8 一樣的 OV framework 及 framework interface version，因此 OV 0.8 的模組，理論上都可以使用。除了 OVIMSpaceChewing 以外的模組，都可以安裝在：

        ~/Library/Application Support/OpenVanilla/Modules/
   
12. 這個版本可以在 Mac OS X 10.6 上使用，不會有左右鍵錯誤的問題。

我們在先前的 mailing list 上，有提過 "Oranje" 分支計畫的目的，在於重寫整套 OS X 版本的 loader，提供一個核心功能的 OpenVanilla。

目前放在 Google Code 上的 OpenVanilla 主幹，同時有 Linux 版本的設定及包裝檔案。Oranje branch 因為希望整理 source tree 的目錄結構，為不影響現有其他版本使用者，因此都只在 github 上的 tree 進行。原有的主幹放在：

    http://code.google.com/p/openvanilla/

我們會慢慢關閉在 Google Code 上關於 Mac 版本的問題回報與發佈檔案。

同時，因為 OpenVanilla Windows 版本自 0.7.2 之後已經停止維護，我們也會移除 Google Code 上所有跟 OV-Win32 有關的檔案。

原有在 OpenFoundry 的 OV 計畫 (http://of.openfoundry.org/projects/209)，我們也會在近期提出完全移除的申請。

上述的移除計畫，預計從台北時間 2009 年 9 月 1 日 14:00 起開始進行。如果還有需要過去舊檔案的朋友，請在此之前下載所需的檔案。

我們的時間可能不足以照顧計畫的每一個環節。如果你覺得這個 open source 有任何可以改善的地方，OpenVanilla 歡迎你的加入！請寄信給 lukhnos {at} lukhnos {dot} org 來取得 github 的 commit 權限。

如果想要從 source code 直接建立這一版的 OpenVanilla ，可以從 github 上取得後，直接使用 OpenVanilla.xcodeproj 建立就可以。請使用 Xcode 3.1 以上版本，並且在 Mac OS X 10.5 或以上的版本建立。建立出來的 OpenVanilla.app 跟 OpenVanilla.bundle 就是分別供 Mac OS X 10.5/10.6 以及 10.4 使用的兩個版本。

如果有任何使用上的問題，可以在 mailing list 上提出，或是透過 github 的簡易 issue tracker 來回報：

    http://github.com/lukhnos/openvanilla-oranje/issues

同樣地，限於時間，我們不一定能處理所有的問題回報，

其他補充說明及注意事項，請參考 github 上的 README:

    http://github.com/lukhnos/openvanilla-oranje/blob/master/README.markdown

謝謝大家！

Lukhnos
