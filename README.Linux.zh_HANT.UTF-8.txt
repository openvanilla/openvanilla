OpenVanilla 0.7.1 / 0.7.2: Framework, Loaders and Modules 
Copyright (c) 2004-2006 The OpenVanilla Project

編譯前需具備以下幾種工具：

g++ 3.x 以上
gmake (gnu make)
libtool
depends on ltdl，請事先安裝

某些模組還需要 libsqlite3。

系統放標頭檔的位置可能不盡相同，請自行修改 Makefile。

用到 syslimits.h 的模組，可能得改用 limits.h。