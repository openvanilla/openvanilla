HDRDIR = ..\..\Framework\Headers
SHRDIR = ..\SharedSource
SHRHDIR = ..\SharedHeaders
CFLAGS = /nologo /I $(HDRDIR) /I $(SHRHDIR) /GR /EHsc /MT /D OV_DEBUG /D WIN32 /c
all: $(SOURCES)
	@link *.obj /dll /def:..\OVModule.def /Out:$(MODULEID).dll
$(SOURCES):
	@if exist $(SHRDIR)\$@.cpp \
		$(CC) $(CFLAGS) $(SHRDIR)\$@.cpp
	@if exist $@.cpp \
		$(CC) $(CFLAGS) $@.cpp
clean:
	-del *.obj $(MODULEID).dll $(MODULEID).lib $(MODULEID).exp