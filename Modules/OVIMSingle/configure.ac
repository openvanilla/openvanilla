#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([openvanilla-module-single],[0.8.0],[matlinuxer2@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER([config.h])

# Check variables
AC_ARG_WITH( cin, AC_HELP_STRING([--with-cin=CIN_FILEPATH], [to use custom cin file]),[CIN=$withval],[CIN=""])
if test -e $CIN; then
cp -v $CIN single.cin
fi

AC_ARG_WITH( name,AC_HELP_STRING([--with-name=MODULE_NAME],[to use custom module name]),[NAME=$withval],[NAME=""])
AC_DEFINE_UNQUOTED( NAME, "$NAME",[name for the single module])
AC_SUBST(NAME)
#AM_CONDITIONAL(WITH_NAME,[test "x$NAME" != "x" ])

AC_ARG_WITH( maxseqlen,AC_HELP_STRING([--with-maxseqlen=MAXLEN],[max length of input seqence]),[MAXSEQLEN=$withval],[MAXSEQLEN="4"])
AC_DEFINE_UNQUOTED( SINGLE_MAXSEQLEN, $MAXSEQLEN,[max length of input seqence])
AC_SUBST(SINGLE_MAXSEQLEN)

AC_ARG_WITH( autocompose,AC_HELP_STRING([--with-autocompose],[to enable autocompose or not]),[AUTOCOMPOSE="1"],[AUTOCOMPOSE="0"])
AC_DEFINE_UNQUOTED( SINGLE_AUTOCOMPOSE, $AUTOCOMPOSE,[to enable autocompose or not])
AC_SUBST(SINGLE_AUTOCOMPOSE)

AC_ARG_WITH( hitmax,AC_HELP_STRING([--with-hitmax],[to enable hitmax or not]),[HITMAX="1"],[HITMAX="0"])
AC_DEFINE_UNQUOTED( SINGLE_HITMAX, $HITMAX,[to enable hitmax or not])
AC_SUBST(SINGLE_HITMAX)

AC_ARG_WITH( shiftselectionkey,AC_HELP_STRING([--with-shiftselectionkey],[to enable shiftselectionkey or not]),[SHIFTSELECTIONKEY="1"],[SHIFTSELECTIONKEY="0"])
AC_DEFINE_UNQUOTED( SINGLE_SHIFTSELECTIONKEY, $SHIFTSELECTIONKEY,[to enable shiftselectionkey or not])
AC_SUBST(SINGLE_SHIFTSELECTIONKEY)


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lsqlite3':
AC_CHECK_LIB([sqlite3], [main])

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h float.h limits.h stdlib.h string.h sys/param.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_CHECK_FUNCS([bzero floor memset munmap select setenv strcasecmp strchr strcspn strspn strstr strtol])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
