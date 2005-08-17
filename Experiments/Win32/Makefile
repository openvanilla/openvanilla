Proj = OVIME
OUTDIR = Result

all: $(OUTDIR)\$(Proj).dll

LINK32_OBJS = \
	      $(OUTDIR)\ControlHandle.obj \
	      $(OUTDIR)\Global.obj \
	      $(OUTDIR)\IME.obj \
	      $(OUTDIR)\NotifyHandle.obj \
	      $(OUTDIR)\OVIME.obj \
	      $(OUTDIR)\UIWnd.obj \
	      $(OUTDIR)\Utils.obj \
              $(OUTDIR)\OVStringToolKit.obj \
	      $(OUTDIR)\OVDummyLoader.obj \
	      $(OUTDIR)\DOVDictionary.obj

$(OUTDIR):
	if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

LINK_LIBS = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib OVIME\IMM32.LIB OVIME\OVIMEUI.lib OVDummyLoader\libltdl.lib OVDummyLoader\libiconv.lib OVDummyLoader\expat\libexpat.lib scew.lib

dllllflags = /INCREMENTAL:NO /NOLOGO /DLL /PDB:$(OUTDIR)/vc70.pdb /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /MACHINE:IX86

$(OUTDIR)\$(Proj).dll: $(OUTDIR) $(LINK32_OBJS) $(OUTDIR)\$(Proj).res
	link $(dllllflags) \
	/NODEFAULTLIB:scew_.lib \
	/LIBPATH:OVDummyLoader\scew \
	$(LINK32_OBJS) $(OUTDIR)\$(Proj).res $(LINK_LIBS) \
	-out:$(OUTDIR)\$(Proj).ime\
	-def:OVIME\$(Proj).def

cflags = /O2 /Ob1 /Oy /I "OVIME" /I "OVIMEUI\Headers" /I "../../Headers" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "OVIME_EXPORTS" /D "_UNICODE" /D "UNICODE" /D "_WINDLL" /GF /EHsc /MT /GS /Gy /W3 /c /Wp64 /Zi /TP 
FILE=ControlHandle
$(OUTDIR)\$(FILE).obj: OVIME\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVIME\$(FILE).cpp
FILE=Global
$(OUTDIR)\$(FILE).obj: OVIME\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVIME\$(FILE).cpp
FILE=IME
$(OUTDIR)\$(FILE).obj: OVIME\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVIME\$(FILE).cpp
FILE=NotifyHandle
$(OUTDIR)\$(FILE).obj: OVIME\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVIME\$(FILE).cpp
FILE=OVIME
$(OUTDIR)\$(FILE).obj: OVIME\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVIME\$(FILE).cpp
FILE=UIWnd
$(OUTDIR)\$(FILE).obj: OVIME\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVIME\$(FILE).cpp
FILE=Utils
$(OUTDIR)\$(FILE).obj: OVIME\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVIME\$(FILE).cpp

cflags = /O2 /Ob1 /Oy /I "OVIME" /I "OVIMEUI\Headers" /I "../../Headers" /I "OVDummyLoader/expat" /I "OVDummyLoader/scew" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "OVIME_EXPORTS" /D "_WINDLL" /GF /EHsc /MT /GS /Gy /W3 /c /Wp64 /Zi /TP /GR
FILE=OVStringToolKit
$(OUTDIR)\$(FILE).obj: ..\..\Source\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" ..\..\Source\$(FILE).cpp
FILE=OVDummyLoader
$(OUTDIR)\$(FILE).obj: OVDummyLoader\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVDummyLoader\$(FILE).cpp
FILE=DOVDictionary
$(OUTDIR)\$(FILE).obj: OVDummyLoader\$(FILE).cpp $(OUTDIR)
	$(CXX) $(cflags) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" OVDummyLoader\$(FILE).cpp

FILE=OVIME
$(OUTDIR)\$(FILE).res: OVIME\$(FILE).rc $(OUTDIR)
	$(RC) $(rcflags) /fo $(OUTDIR)\$(FILE).res OVIME\$(FILE).rc

clean:
	rmdir /S /Q $(OUTDIR)
