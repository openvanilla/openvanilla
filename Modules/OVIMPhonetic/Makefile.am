libdir=$(prefix)/lib/openvanilla/

lib_LTLIBRARIES = libovimphonetic.la
libovimphonetic_la_SOURCES = OVIMPhoneticStatic.cpp OVPhoneticData.c
libovimphonetic_la_LDFLAGS = -lsharedsource
libovimphonetic_la_LIBADD = -L../SharedSource/
libovimphonetic_la_CPPFLAGS = -I$(top_srcdir)/SharedHeaders/

ovimphoneticdir=$(libdir)/OVIMPhonetic/
ovimphonetic_DATA= bpmf-ext.cin bpmf.cin punctuations.cin

EXTRA_DIST=  bpmf-sqlconvert.pl convert-phonedata.pl bpmf-ext.cin bpmf.cin punctuations.cin
