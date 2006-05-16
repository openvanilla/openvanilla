MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/src/porting_layer/include

noinst_LTLIBRARIES = libporting_layer.la
libporting_layer_la_SOURCES = \
	plat_mmap_posix.c \
	plat_mmap_win32.c
