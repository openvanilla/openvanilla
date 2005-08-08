請用這裡的 vcproj 來做出 DLL，或用 VC 2003 的 cl.exe 搭配 make.bat 來做。

要特別注意 OVIMTibetan.cpp 雖然一樣使用 UTF-8，卻必須有 BOM (Byte-Order Mark)。
而且 VS.NET 2003 很神奇地會把 BOM 洗掉，讓它自己下次讀進來的時候誤判...
想改 OVIMTibetan.cpp，建議用 EmEditor 或 UltraEditor。