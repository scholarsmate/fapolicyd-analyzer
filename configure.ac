#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE(fapolicy-analyzer,0.1)
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

m4_define([SQLITE_REQUIRED_VERSION], [3.11])

#AX_CFLAGS_WARN_ALL(C_WFLAGS)
#AX_CXXFLAGS_WARN_ALL(CXX_WFLAGS)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
LT_INIT
AC_CHECK_PROG(SQLITE3_CHECK,sqlite3,yes)
if test x"$SQLITE3_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install sqlite3 binary.])
fi

# Checks for libraries.
if test "x$SQLITE3_CFLAGS$SQLITE3_LIBS" = "x" ; then
    PKG_CHECK_MODULES([SQLITE3], [sqlite3 >= SQLITE_REQUIRED_VERSION])
fi
AC_SUBST(SQLITE3_CFLAGS,$SQLITE3_CFLAGS)
AC_SUBST(SQLITE3_LIBS,$SQLITE3_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
