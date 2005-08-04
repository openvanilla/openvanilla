到 trunk/Modules/OVIMTobacco/ 執行 make.bat 就能做出 DLL。
或是使用本目錄中的 vcproj 也行。

至於做出 imtables.db 和 tsi.db 的方式，如果要照著 Makefile 裡的步驟來，當然是要有 ActivePerl，
另外還要把 http://www.sqlite.org/sqlite-3_2_2.zip 裡的 sqlite3.exe 放在同一目錄下。