到 trunk/Modules/OVIMTobacco/ 底下建一個叫 sqlite-src 的目錄, 
把 http://www.sqlite.org/sqlite-source-3_2_2.zip 解壓縮放到裡面，但拿掉 tclsqlite.c。

然後再執行 make.bat 就能做出 DLL。

至於做出 imtables.db 和 tsi.db 的方式，如果要照著 Makefile 裡的步驟來，當然是要有 ActivePerl，
另外還要把 http://www.sqlite.org/sqlite-3_2_2.zip 裡的 sqlite3.exe 放在同一目錄下。