AUTOMAKE_OPTIONS=foreign
libdir=$(prefix)/lib/openvanilla/

SUBDIRS = SharedSource SharedHeaders

lib_LTLIBRARIES = libovim@NAME@.la
libovim@NAME@_la_SOURCES = CIN-Defaults.cpp OVIMSingle.cpp CIN-Defaults.h OVIMSingle.h
libovim@NAME@_la_LDFLAGS = -lsharedsource
libovim@NAME@_la_LIBADD = -L./SharedSource/
libovim@NAME@_la_CPPFLAGS = -I$(top_srcdir)/SharedHeaders/

ovimsingledir=$(libdir)/OVIM@NAME@/
ovimsingle_DATA=single.cin

EXTRA_DIST=$(ovimsingle_DATA)
