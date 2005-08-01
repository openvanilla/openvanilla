=========================================================================
    延伸預存程序 : OVDummyLoader 專案概觀
========================================================================


AppWizard 已經為您建立了 OVDummyLoader.dll。

這個檔案含有構成 OVDummyLoader 應用程式之每一個檔案的內容摘要。

OVDummyLoader.vcproj
    這是使用應用程式精靈所產生之 VC++ 專案的主要專案檔。
    它含有產生該檔案之 Visual C++  的版本資訊，以及有關使用應用程式精靈所選取之平台、組態和專案功能的資訊。

OVDummyLoader.cpp
    這是主要 DLL 原始程式檔。

proc.cpp
    這個檔案含有預存程序 xp_proc

/////////////////////////////////////////////////////////////////////////////
其他標準檔案:

StdAfx.h, StdAfx.cpp
    這些檔案是用來建置名為 OVDummyLoader.pch 的先行編譯標頭 (PCH) 檔
    和名為 StdAfx.obj 的先行編譯型別檔。


/////////////////////////////////////////////////////////////////////////////
其他注意事項:

完成這個精靈之後，請複製 OVDummyLoader.dll 到您的 SQL Server 
\Binn 目錄。

從 Visual Studio 資料專案，或使用 SQL Server Enterprise Manager，或執行下列 SQL 命令:
  sp_addextendedproc 'xp_proc', 'OVDummyLoader.DLL'
  加入新的延伸預存程序

您可以使用 SQL 命令:
  sp_dropextendedproc 'xp_proc'
  來卸除延伸預存程序

您可以使用 SQL 命令:
  DBCC xp_proc(FREE)
從伺服器釋放 DLL (刪除或取代檔案) 


/////////////////////////////////////////////////////////////////////////////
