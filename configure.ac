dnl Process this file with autoconf to produce a configure script.
dnl Created by Sébastien Le Roux.

AC_PREREQ(2.59)

m4_define(atomes_major_version, 1)
m4_define(atomes_minor_version, 1)
m4_define(atomes_patch_version, 17)
m4_define(atomes_version, atomes_major_version.atomes_minor_version.atomes_patch_version)
m4_define(atomes_url, https://atomes.ipcms.fr)
m4_define(atomes_tar, atomes)

AC_INIT([atomes], [atomes_version], [atomes@ipcms.unistra.fr], [atomes_tar], [atomes_url])

AM_INIT_AUTOMAKE

AC_DEFINE([ATOMES_MAJOR_VERSION], [atomes_major_version], [atomes major version])
AC_SUBST(ATOMES_MAJOR_VERSION, atomes_major_version)
AC_DEFINE([ATOMES_MINOR_VERSION], [atomes_minor_version], [atomes minor version])
AC_SUBST(ATOMES_MINOR_VERSION, atomes_minor_version)
AC_DEFINE([ATOMES_PATCH_VERSION], [atomes_patch_version], [atomes patch version])
AC_SUBST(ATOMES_PATCH_VERSION, atomes_patch_version)

dnl ***************************************************************************
dnl Internatinalization
dnl ***************************************************************************

GETTEXT_PACKAGE=atomes
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])

dnl ***************************************************************************
dnl Check for the operating system
dnl ***************************************************************************

AC_CANONICAL_HOST

build_linux=no
build_windows=no
build_mac=no

# Detect the target system
case "${host_os}" in
    linux*)
        build_linux=yes
	echo "Building Linux application"
        ;;
    cygwin*|mingw*)
        build_windows=yes
	echo "Building Windows application"
        ;;
    darwin*)
        build_mac=yes
	echo "Building macOS application"
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac

# Pass the conditionals to automake
AM_CONDITIONAL([LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([WINDOWS], [test "$build_windows" = "yes"])
AM_CONDITIONAL([OSX], [test "$build_mac" = "yes"])

if test "$build_windows" = "yes"; then
  AC_CHECK_TOOL([WINDRES], [windres], [no])
  if test "$WINDRES" = no; then
    AC_MSG_ERROR([*** Could not find an implementation of windres in your PATH.])
  fi
fi

dnl ***************************************************************************
dnl Check for utilities
dnl ***************************************************************************
AC_CHECK_PROG([PKG_CONFIG], [pkg-config], [yes], [no])
AC_CHECK_PROG([UP_MIME], [update-mime-database], [yes], [no])
AC_CHECK_PROG([UP_DESKTOP], [update-desktop-database], [yes], [no])
AC_CHECK_PROG([UP_APPSTREAM], [appstream-util], [yes], [no])


dnl ***************************************************************************
dnl Check for required librairies
dnl ***************************************************************************

# determine the GTK version to use
AC_MSG_CHECKING([the GTK version to use])
AC_ARG_WITH([gtk],
        [AS_HELP_STRING([--with-gtk=3|4], [the GTK version to use (default: 3)])],
        [case "$with_gtk" in
	        3|4) ;;
        	*) AC_MSG_ERROR([invalid GTK version specified]) ;;
        esac],
        [with_gtk=3])
AC_MSG_RESULT([$with_gtk])
AM_CONDITIONAL([GTK3], [test "$with_gtk" == "3"])
case "$with_gtk" in
	3) gtk_version="gtk+-3.0"
	   gtk_version_number="3.16"
	   ;;
	4) gtk_version="gtk4"
	   gtk_version_number="4.6"
   	   ;;
esac	   
PKG_CHECK_MODULES([LGTK], [$gtk_version >= $gtk_version_number])
if test "$with_gtk" = "3"; then
	if test "$build_mac" = "yes"; then
		PKG_CHECK_MODULES([INTEGRATION], [gtk-mac-integration])
	fi
fi
PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= 2.4.0])
PKG_CHECK_MODULES([PANGOFT2], [pangoft2])
PKG_CHECK_MODULES([FFMPEG], [libavcodec libavformat libavutil libswscale])
if test "$build_windows" = "no"; then
  PKG_CHECK_MODULES([GLU], [glu])
fi
PKG_CHECK_MODULES([EPOXY], [epoxy])

# Checking for compilers
dnl Definig the macro AX_CHECK_COMPILER_FLAG:
AC_DEFUN([AX_CHECK_COMPILER_FLAG],
  [AC_PREREQ(2.64) dnl for _AC_LANG_PREFIX and AS_VAR_IF
  AS_VAR_PUSHDEF([CACHEVAR], [ax_cv_check_[]_AC_LANG_ABBREV[]flags_$4_$1])
  AC_CACHE_CHECK([whether _AC_LANG compiler accepts $1], CACHEVAR, [
    ax_check_save_flags=$[]_AC_LANG_PREFIX[]FLAGS
    _AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS $4 $1"
    AC_COMPILE_IFELSE([m4_default([$5],[AC_LANG_PROGRAM()])],
                      [AS_VAR_SET(CACHEVAR,[yes])],
                      [AS_VAR_SET(CACHEVAR,[no])])
    _AC_LANG_PREFIX[]FLAGS=$ax_check_save_flags])
    AS_VAR_IF(CACHEVAR,yes,
      [m4_default([$2], :)],
      [m4_default([$3], :)])
  AS_VAR_POPDEF([CACHEVAR])
])dnl AX_CHECK_COMPILER_FLAG

# C
AC_PROG_CC

AX_CHECK_COMPILER_FLAG([$CFLAGS])

# Fortran 90 

AC_FC_WRAPPERS

AC_LANG_PUSH([Fortran])
fcflags_save="$FCFLAGS"
AC_PROG_FC([xlf95 fort ifort ifc efc f95 g95 pgf95 lf95 ftn xlf90 f90 pgf90 pghpf epcf90 gfortran gfortran-4 gfortran-4.exe], [90])
FCFLAGS="$fcflags_save"

AC_FC_SRCEXT(f90, FCFLAGS_f90="$FCFLAGS_f90 $FCFLAGS", AC_MSG_ERROR([Cannot compile .f90 files]))
AC_FC_SRCEXT(F90, FCFLAGS_F90="$FCFLAGS_F90 $FCFLAGS", AC_MSG_ERROR([Cannot compile .F90 files]))

AX_CHECK_COMPILER_FLAG([$FCFLAGS])

AC_FC_LIBRARY_LDFLAGS
AC_FC_FREEFORM
AC_LANG_POP([Fortran])

cat lic > src/version.h
echo "#define FC \""$FC"\"" >> src/version.h
echo "#define FCFLAGS \""$FCFLAGS"\"" >> src/version.h
echo "#define CC \""$CC"\"" >> src/version.h
echo "#define CFLAGS \""$CFLAGS"\"" >> src/version.h

AC_MSG_CHECKING([the OpenMP specification and flags])
AC_ARG_ENABLE([--disable-openmp], [AS_HELP_STRING([--disable-openmp], [disable OpenMP thread parallelisation (default: no)])], [with_opemp="no"], [with_openmp="yes"])
AC_MSG_RESULT([$with_openmp])
if test "$with_openmp" = "yes"; then
  AC_OPENMP
  AC_SUBST([OPENMP_CFLAGS])
  AM_CONDITIONAL([OPENMP], [test "${OPENMP_CFLAGS}" != ""])
fi

AC_CONFIG_FILES([
 Makefile
 data/Makefile
 src/Makefile
 src/startup-testing/Makefile 
])

AC_CONFIG_HEADERS([config.h])
if test "$build_windows" = "yes"; then
  AC_CONFIG_FILES([misc/win/atomes.rc:misc/win/atomes.rc.in misc/win/startup.rc:misc/win/startup.rc.in])
fi
AC_OUTPUT
