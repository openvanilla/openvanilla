OpenVanilla 0.7.2 (beta) for OS X
大易輸入法資料表格補充版安裝說明

Copyright (c) 2004-2006 The OpenVanilla Project

這個套件內含一份修正過的 dayi3.cin ，更動過的部份有：

* 加入一批標點符號和一個新的 keyname (' 標)
* 改變二鍵輸入時的一些常用字選字順序：
  也道爸就像身假流遠應漢蘭動崗亂病碗獅據遊眾萬灣愈淨管舒陽制夢

安裝方法有兩種，可以將 dayi3.cin 拖進以下任何一個目錄中：

* 自己家目錄下「資源庫」裡的 OpenVanilla/0.7.2/UserSpace/OVIMGeneric/
  用 UNIX 的目錄寫法是 ~/Library/OpenVanilla/0.7.2/UserSpace/OVIMGeneric/
  
* 拉進主硬碟「資源庫」裡的 OpenVanilla/0.7.2/Modules/OVIMGeneric/
  用 UNIX 的目錄寫法是 /Library/OpenVanilla/0.7.2/Modules/OVIMGeneric/
  用這個方法會蓋寫掉原有目錄裡，OV標準安裝包附的大易資料表，
  蓋寫前Finder會跟你要管理者權限。如果用 UNIX 的 cp 命令，請加 sudo。

如果是安裝在自己家目錄中，OV 會以家目錄的資料表優先，已經載入家目錄下的
dayi3.cin，就不會載入系統預設的同名資料表了。
