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

osx-framework: OpenVanilla.h OVLoadable.h
	mkdir -p $(OSXFRAMEWORK)/Headers
	touch $(OSXFRAMEWORK)/OpenVanilla
	cp OpenVanilla.h OVLoadable.h $(OSXFRAMEWORK)/Headers


