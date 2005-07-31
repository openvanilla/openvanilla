1. 先到 DummyLoader 目錄下執行 make.bat
2. 把 libltdl3.dll 和 OVDummyLoader.DLL 放到 C:\Windows\system32
3. 到 Ime 裡編出 OVIME.dll
4. 到 Modules 目錄下依照 README.txt 的指示編出對應的 DLL，在此以 OVIMPOJ-Holo 為例
5. 把 OVIMPOJ-Holo.DLL 放到 C:\OpenVanilla\ 底下，這是 kanru 暫時指定的 module 目錄
6. 把 poj-holo.cin 放到 C:\OpenVanilla\OVIMPOJ-Holo 裡
7. 把 OVIME.dll 更名為 OVIME.ime，複製到 C:\Windows\system32 下
8. 執行 OVIME.reg
8. 開啟輸入法設定視窗, 新增剛剛裝入的輸入法