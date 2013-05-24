dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(cppcontainers, 0.1)

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CPP
AC_PROG_CXX
AC_PROG_LIBTOOL


dnl
dnl Tests for CppUnit availability and usability
dnl Using CppUnit implies C++ support!
dnl Theoretically, libcppunit-dev will pull up to g++, through libstdc++...
dnl AM_PATH_CPPUNIT(1.9.6)

PKG_CHECK_MODULES(CPPUNIT, cppunit, have_cppunit=yes, have_cppunit=no)
if test "${have_cppunit}" != "yes" ; then
	AC_MSG_WARN([libcppunit not found.])
fi
AM_CONDITIONAL(HAVE_CPPUNIT, test "${have_cppunit}" = "yes")






AC_OUTPUT([
include/Makefile
tests/Makefile
Makefile
])
