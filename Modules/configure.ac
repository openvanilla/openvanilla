#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Here thanks for the autotool teaching from Fourdollars <fourdollars@gmail.com> 

AC_PREREQ(2.61)
AC_INIT([openvanilla-modules],[0.8.0_7],[ucimf@googlegroups.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADER([config.h])

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

AC_CONFIG_FILES([Makefile
                 OVIMGeneric/Makefile
                 SharedData/Makefile
                 SharedHeaders/Makefile
                 SharedSource/Makefile])

AC_OUTPUT
