OS=`uname`

all:
	make -f Makefile.$(OS) $@

install:
	make -f Makefile.$(OS) $@

uninstall:
	make -f Makefile.$(OS) $@

clean:
	make -f Makefile.$(OS) $@

