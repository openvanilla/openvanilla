SRCDIR=Source
HDRDIR=Header
HEADERS=$(HDRDIR)/OpenVanilla.h $(HDRDIR)/OVLoadable.h $(HDRDIR)/OVUtility.h

BUILDDIR=build
OSXFRAMEWORK=$(BUILDDIR)/OpenVanilla.framework
GOALS=osx-framework

build: osx-framework

install: install-osx-framework

clean:
	rm -rf $(BUILDDIR)

# OSX-Specific targets

install-osx-framework: osx-framework
	cp -rf $(OSXFRAMEWORK) /Library/Frameworks/

osx-framework:
	mkdir -p $(OSXFRAMEWORK)/Headers
	cp $(HEADERS) $(OSXFRAMEWORK)/Headers

