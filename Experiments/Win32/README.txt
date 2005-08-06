1. 用 Visual Studio .NET 2003 開啟 OVIME.sln，按需求選擇 Debug 或 Release 模式，執行「建置方案」。
2. 執行 deploy-debug.bat 或 deploy-release.bat。
3. 到 Modules 目錄下依照 README.txt 的指示編出對應的 DLL，在此以 OVIMPOJ-Holo 為例
4. 把 OVIMPOJ-Holo.DLL 放到 %WINDIR%\OpenVanilla\ 底下，這是 kanru 暫時指定的 module 目錄
5. 把 poj-holo.cin 放到 %WINDIR%\OpenVanilla\OVIMPOJ-Holo 裡
6. 執行 OVIME.reg
7. 開啟輸入法設定視窗, 新增剛剛裝入的輸入法