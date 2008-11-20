AUTOMAKE_OPTIONS=foreign
libdir=$(prefix)/lib/openvanilla/


lib_LTLIBRARIES = libovim@NAME@.la
libovim@NAME@_la_SOURCES = CIN-Defaults.cpp OVIMSingle.cpp
libovim@NAME@_la_LDFLAGS = -lsharedsource
libovim@NAME@_la_LIBADD = -L../SharedSource/
libovim@NAME@_la_CPPFLAGS = -I../SharedHeaders/

ovimsingledir=$(libdir)/OVIM@NAME@/
ovimsingle_DATA=single.cin

if WITH_CIN
install-data-hook:
	cp -v @CIN@ single.cin
endif
