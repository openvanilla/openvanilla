bin_PROGRAMS=analyze chmorph example hunspell munch unmunch

INCLUDES=-I${top_srcdir}/src/hunspell -I${top_srcdir}/src/parsers

munch_SOURCES=munch.c
unmunch_SOURCES=unmunch.c
include_HEADERS=munch.h unmunch.h

example_SOURCES=example.cxx
example_LDADD = ../hunspell/libhunspell.la

hunspell_SOURCES=hunspell.cxx
hunspell_LDADD = @LIBINTL@ ../hunspell/libhunspell.la \
	../parsers/libparsers.a @CURSESLIB@ @READLINELIB@

analyze_SOURCES=analyze.cxx
analyze_LDADD = ../hunspell/libhunspell.la

chmorph_SOURCES=chmorph.cxx
chmorph_LDADD = ../hunspell/libhunspell.la ../parsers/libparsers.a

EXTRA_DIST=makealias
