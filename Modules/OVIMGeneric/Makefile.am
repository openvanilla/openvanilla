libdir=$(prefix)/lib/openvanilla/

lib_LTLIBRARIES = libovimgeneric.la
libovimgeneric_la_SOURCES = CIN-Defaults.cpp OVIMGeneric.cpp
libovimgeneric_la_LDFLAGS = -lsharedsource
libovimgeneric_la_LIBADD = -L../SharedSource/
libovimgeneric_la_CPPFLAGS = -I$(top_srcdir)/SharedHeaders/

ovimgenericdir=$(libdir)/OVIMGeneric/
ovimgeneric_DATA= cj.cin dayi3.cin ehq-symbols.cin pinyin.cin simplex.cin wubizixing.cin

EXTRA_DIST= CIN-Defaults.h OVIMGeneric.h \
		 cj.cin dayi3.cin ehq-symbols.cin pinyin.cin simplex.cin wubizixing.cin
