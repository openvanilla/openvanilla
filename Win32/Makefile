OUTDIR = Build
OVHDRDIR = ../../Framework/Headers
SHDRDIR = ../../Modules/SharedHeaders
LINK_LIBS = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \
            advapi32.lib comdlg32.lib comctl32.lib shell32.lib ole32.lib \
			oleaut32.lib uuid.lib imm32.lib Ws2_32.lib AmphiVanilla\libltdl.lib \
            AmphiVanilla\libiconv.lib tinyxml\tinyxml.lib

HDRFLAGS = /I OVIME /I OVIME\UI /I AmphiVanilla /I $(OVHDRDIR) /I $(SHDRDIR) /I tinyxml
DEFFLAGS = /D WIN32 /D NDEBUG /D _WINDOWS /D _USRDLL /D OVIME_EXPORTS /D _WINDLL
# /Ob1 = inline expansion
# /Oy = enable frame pointer omission
# /GF = enable read only string pooling
# /EHsc =  enable C++ EH (no SEH exceptions), extern "C" defaults to nothrow
# /MT = link with LIBCMT.LIB
# /GS = enable security checks
# /Gy = separate functions for linker
# /W3 = warning level
# /c = compile only
# /Wp64 = enable 64 bit porting warnings
# /Zi = enable debugging information
# /TP = compile all files as .cpp
# /GR = enable c++ RTTI
# /Fo = obj file output dir
CFLAGS = $(HDRFLAGS) $(DEFFLAGS) /nologo /wd4005 /wd4996 /O1 /Ob1 /Oy /GF /EHsc \
		 /MT /GS /Gy /W3 /c /Wp64 /Zi /TP /GR /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\"
UNICODE = /D "_UNICODE" /D "UNICODE"
DLLFLAGS = /INCREMENTAL:NO /nologo /dll /PDB:$(OUTDIR)/vc80.pdb /SUBSYSTEM:WINDOWS \
           /OPT:REF /OPT:ICF /MACHINE:IX86
AVDIR = AmphiVanilla
AVOBJS = AVConfig.obj AVBuffer.obj AVCandidate.obj AVEmbeddedLoader.obj \
         AVLoader.obj AVLoaderUtility.obj AVDictionary.obj AVService.obj
OVIMEDIR = OVIME
OVIMEOBJS = ControlHandle.obj Global.obj IME.obj NotifyHandle.obj OVIME.obj \
            UIWnd.obj Utils.obj DisplayServer.obj
OVIMEUIDIR = OVIME\UI
OVIMEUIOBJS = UIGlobal.obj PCMan.obj UICand.obj UIComp.obj UINotify.obj UIStatus.obj UIUtils.obj
# AmphiVanilla OVIME OVIMEUI
all: AmphiVanilla OVIME OVIMEUI
	link $(DLLFLAGS) $(OUTDIR)\*.obj \
	$(OUTDIR)\OVIME.res $(LINK_LIBS) \
	-out:$(OUTDIR)\OVIME.ime \
	-def:OVIME\OVIME.def
AmphiVanilla: $(OUTDIR) $(AVOBJS)
OVIME: $(OUTDIR) $(OVIMEOBJS)
	$(RC) /fo $(OUTDIR)\OVIME.res OVIME\OVIME.rc
OVIMEUI: $(OUTDIR) $(OVIMEUIOBJS)
$(AVOBJS):
	@$(CC) $(CFLAGS) $(AVDIR)\$*.cpp 
$(OVIMEOBJS):
	@$(CC) $(CFLAGS) $(UNICODE) /D "_WINSOCKAPI_" /D "_WINDLL" $(OVIMEDIR)\$*.cpp
$(OVIMEUIOBJS):
	@$(CC) $(CFLAGS) $(UNICODE) $(OVIMEUIDIR)\$*.cpp
$(OUTDIR):
	@if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)
clean:
	rmdir /S /Q $(OUTDIR)