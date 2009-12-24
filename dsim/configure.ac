# Process this file with autoconf to produce a configure script
#-------------------------------------------------------------------
AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (C) 2007-2010 Amadeus DEV-ORI <darnaud@amadeus.com>])
AC_INIT([DSIM],[0.1.0],[darnaud@amadeus.com],[dsim])
AC_CONFIG_HEADER([stdair/config.h])
AC_CONFIG_SRCDIR([stdair/basic/BasConst.cpp])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])
AM_INIT_AUTOMAKE
AM_PATH_CPPUNIT(1.10)
AM_CONDITIONAL([HAVE_CPPUNIT], [test "x$CPPUNIT_LIBS" != x])

# RPM release number
RPM_RELEASE="1"
AC_SUBST(RPM_RELEASE)

# Shared library versioning for the main library
GENERIC_LIBRARY_VERSION="2:1:0"
#                        | | |
#                 +------+ | +---+
#                 |        |     |
#              current:revision:age
#                 |        |     |
#                 |        |     +- increment if interfaces have been added;
#                 |        |        set to zero if interfaces have been 
#                 |        |        removed or changed
#                 |        +- increment if source code has changed;
#                 |           set to zero if current is incremented
#                 +- increment if interfaces have been added, removed
#                    or changed
AC_SUBST(GENERIC_LIBRARY_VERSION)

# Upload command
UPLOAD_COMMAND="ncftpput -u ori-data -v orinet.nce.amadeus.net /remote/oridata/www/oridist/projects/dsim/$VERSION"
AC_SUBST(UPLOAD_COMMAND)

# Checks for programs
save_CXXFLAGS="$CXXFLAGS"
AC_PROG_CXX
AC_PROG_CXXCPP
CXXFLAGS="$save_CXXFLAGS"
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Check for host and disable building a shared library in Windows
AC_CANONICAL_HOST
case $host in
  *-*-msdos* | *-*-go32* | *-*-mingw32* | *-*-cygwin* | *-*-windows*) 
#    AC_DISABLE_SHARED
#    AC_ENABLE_STATIC
    ;;
  *)  
#    AC_ENABLE_SHARED
#    AC_DISABLE_STATIC
    ;;
esac

# Set default language to C++
AC_LANG([C++])

# Check for debug switch
AC_ARG_ENABLE(debug,
              [AS_HELP_STRING([--enable-debug],
                              [build additional debugging library])])
test "x$enable_debug" != xyes && enable_debug=no
AM_CONDITIONAL([ENABLE_DEBUG], [test "x$enable_debug" = xyes])

# Check for exceptions switch
AC_ARG_ENABLE(exceptions,
              [AS_HELP_STRING([--enable-exceptions],
                              [enable exceptions handling mechanizm])],
              [AC_DEFINE([DSIM_EXCEPTIONS], [1],
                         [Define if you want exceptions handling])])
test "x$enable_exceptions" != xyes && enable_exceptions=no
test "x$ax_cv_boost_date_time" != xno && enable_exceptions=yes


# Check for info pages
AC_ARG_ENABLE(info-doc, 
              [AS_HELP_STRING([--disable-info-doc],
                              [do not generate Info documentation])])
test "x$enable_info_doc" != xno && enable_info_doc=yes


# Check for html-doc switch
AC_ARG_ENABLE(html-doc, 
              [AS_HELP_STRING([--disable-html-doc],
                              [do not generate HTML documentation])])
test "x$enable_html_doc" != xno && enable_html_doc=yes


# Provide --with-docdir
AC_ARG_WITH(docdir,
            [AS_HELP_STRING([--with-docdir=DIR],
                            [set documentation directory to DIR])],
            [DOC_DIR="$withval"],
            [DOC_DIR=""])
if test "x$DOC_DIR" != x; then
  docdir="$DOC_DIR"
else
  docdir="${datadir}/doc/$PACKAGE-$VERSION"
fi
AC_SUBST(docdir)


PACKAGE_DEBUG="$PACKAGE"
# Set debug flags
if test "x$enable_debug" = xyes; then
  PACKAGE_DEBUG="$PACKAGE"_debug
  CXXFLAGS_DEBUG="-DASSERT_LEVEL=2 -g"
  # Only for GCC compiler 
  if test "x$GXX" = xyes; then
    if test "x$enable_exceptions" = xno; then
      CXXFLAGS_DEBUG="$CXXFLAGS_DEBUG -fno-exceptions"
    fi
    CXXFLAGS_DEBUG="$CXXFLAGS_DEBUG -Wall -pipe"
  fi
fi
AC_SUBST(CXXFLAGS_DEBUG)
AC_SUBST(PACKAGE_DEBUG)

# Set optimised flags
if test "x$CXXFLAGS" = x; then
  CXXFLAGS_OPT="-DASSERT_LEVEL=1 -O2" 
  # Only for GCC compiler 
  if test "x$GXX" = xyes; then
    if test "x$enable_exceptions" = xno; then
      CXXFLAGS_OPT="$CXXFLAGS_OPT -fno-exceptions"
    fi
    CXXFLAGS_OPT="$CXXFLAGS_OPT -Wall -pipe"
  fi
else
  CXXFLAGS_OPT="$CXXFLAGS"
  CXXFLAGS=""
fi
AC_SUBST(CXXFLAGS_OPT)

# -----------------------------------------------------------------------------
# Support for ExtraCC (Extra-CruiseControl): http://sf.net/projects/extracc
# -----------------------------------------------------------------------------
AM_PATH_EXTRACC
#AC_SUBST(EXTRACC_VERSION)
#AC_SUBST(EXTRACC_CFLAGS)
#AC_SUBST(EXTRACC_LIBS)

# -----------------------------------------------------------------------------
# Support for the GSL (GNU Scientific Library): http://www.gnu.org/software/gsl
# -----------------------------------------------------------------------------
# Provide --with-gsl
AM_PATH_GSL([1.8])
AC_SUBST(GSL_VERSION)
AC_SUBST(GSL_CFLAGS)
AC_SUBST(GSL_LIBS)

# -----------------------------------------------------------
# Python: http://www.python.org
# -----------------------------------------------------------
PGAC_CHECK_PYTHON_EMBED_SETUP
AC_SUBST(PYTHON_VERSION)
AC_SUBST(PYTHON_LIBS)
AC_SUBST(PYTHON_CFLAGS)
AC_SUBST(PYTHON_ADD_LIBS)

# -----------------------------------------------------------
# MPICH2: http://www.mcs.anl.gov/research/projects/mpich2
# Note: Boost.MPI depends on MPICH2, rather than on OpenMPI
# -----------------------------------------------------------
AX_MPICH2
AC_SUBST(MPICH2_VERSION)
AC_SUBST(MPICH2_CFLAGS)
AC_SUBST(MPICH2_LIBS)

# -----------------------------------------------------------
# OpenMPI: http://www.open-mpi.org
# -----------------------------------------------------------
#AX_OPENMPI
AC_SUBST(OPENMPI_VERSION)
AC_SUBST(OPENMPI_CFLAGS)
AC_SUBST(OPENMPI_LIBS)

# ---------------------------------------------------------------
# Support for Boost (Extension of the STL): http://www.boost.org
# ---------------------------------------------------------------
AX_BOOST([1.33])
AC_SUBST(BOOST_VERSION)
AC_SUBST(BOOST_CFLAGS)
AC_SUBST(BOOST_LIBS)
AC_SUBST(BOOST_DATE_TIME_LIB)
AC_SUBST(BOOST_PROGRAM_OPTIONS_LIB)
AC_SUBST(BOOST_FILESYSTEM_LIB)
AC_SUBST(BOOST_THREAD_LIB)
AC_SUBST(BOOST_PYTHON_LIB)
AC_SUBST(BOOST_SIGNALS_LIB)
AC_SUBST(BOOST_ASIO_LIB)
AC_SUBST(BOOST_MPI_LIB)
AC_SUBST(BOOST_MPI_PYTHON_LIB)

# --------------------------------------------------------------------
# Support for MySQL++ (C++ client API): http://tangentsoft.net/mysql++
# --------------------------------------------------------------------
AX_MYSQLPP
AC_SUBST(MYSQLPP_VERSION)
AC_SUBST(MYSQLPP_CFLAGS)
AC_SUBST(MYSQLPP_LIBS)

# --------------------------------------------------------------------
# Support for SOCI http://soci.sourceforge.net
# --------------------------------------------------------------------
AX_SOCI
AC_SUBST(SOCI_VERSION)
AC_SUBST(SOCI_CFLAGS)
AC_SUBST(SOCI_LIBS)

# --------------------------------------------------------------------
# Support for TRAVEL-CCM (CCM Open Library): http://sf.net/projects/travel-ccm
# --------------------------------------------------------------------
AM_PATH_TRAVELCCM
AC_SUBST(TRAVELCCM_VERSION)
AC_SUBST(TRAVELCCM_CFLAGS)
AC_SUBST(TRAVELCCM_LIBS)

# --------------------------------------------------------------------
# Support for RMOL (RM Open Library): http://sf.net/projects/rmol
# --------------------------------------------------------------------
AM_PATH_RMOL([0.19])
AC_SUBST(RMOL_VERSION)
AC_SUBST(RMOL_CFLAGS)
AC_SUBST(RMOL_LIBS)

# ============ X E R C E S ==================
AX_XERCES_C

#
# librt
#
AC_CHECK_LIBRT

# -----------------------------------------------------------
# Xapian (Indexing & Search API: http://www.xapian.org
# -----------------------------------------------------------
XO_LIB_XAPIAN
AC_SUBST(XAPIAN_VERSION)
AC_SUBST(XAPIAN_CFLAGS)
AC_SUBST(XAPIAN_LIBS)

# -------------------------------------------------------------------
# Support for documentation
# -------------------------------------------------------------------
# Checks for documentation build tools
#AC_CHECK_PROG([texinfo_ok], [texinfo], [yes], [no])
#if test "x$enable_info_doc" != xno; then
#  test "x$texinfo_ok" != xyes && enable_info_doc=no
#  AC_PATH_PROG([PERL], [perl], [])
#fi
AM_CONDITIONAL([INFO_DOC], [test "x$enable_info_doc" = xyes])

# Checks for documentation build tools
AC_CHECK_PROG([doxygen_ok], [doxygen], [yes], [no])
AC_CHECK_PROG([latex_ok], [latex], [yes], [no])
AC_CHECK_PROG([dvips_ok], [dvips], [yes], [no])
AC_CHECK_PROG([gs_ok], [gs], [yes], [no])
if test "x$enable_html_doc" != xno; then
  test "x$doxygen_ok" != xyes && enable_html_doc=no
  test "x$latex_ok" != xyes && enable_html_doc=no
  test "x$dvips_ok" != xyes && enable_html_doc=no
  test "x$gs_ok" != xyes && enable_html_doc=no
  AC_PATH_PROG([PERL], [perl], [])
fi
AM_CONDITIONAL([HTML_DOC], [test "x$enable_html_doc" = xyes])


# Check for diff program used for tests
AC_CHECK_PROG([diff_ok], [diff], [yes], [no])
AC_CHECK_PROG([sed_ok], [sed], [yes], [no])
enable_tests=yes
test "x$diff_ok" != xyes && enable_tests=no
test "x$sed_ok" != xyes && enable_tests=no
AM_CONDITIONAL([RUN_TESTS], [test "x$enable_tests" = xyes])

# Checks for header files
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([cmath \
  deque \
  fstream \
  ieeefp.h \
  iomanip \
  iostream \
  sstream \
  limits \
  list \
  vector \
  map \
  queue \
  stdexcept \
  string])


# Checks for global declarations
AC_CHECK_DECLS(signgam, , , [#include <cmath>])


# Checks for functions
AC_CHECK_FUNCS([erf \
  erfc \
  finite \
  fpclass \
  isfinite \
  isinf \
  isnan \
  lgamma \
  log1p \
  rint \
  tgamma])


# Check for finite and fpclass on Solaris
AC_CHECK_FUNC(finite, ,
  [AC_CHECK_FUNC(isfinite, ,
    [AC_TRY_LINK([[#include <ieeefp.h>]], [[float f = 0.0; isfinite(f);]],
      [AC_DEFINE(HAVE_ISFINITE)])])])
AC_CHECK_FUNC(fpclass, ,
  [AC_TRY_LINK([[#include <ieeefp.h>]], [[float f = 0.0; fpclass(f);]],
    [AC_DEFINE(HAVE_FPCLASS)])])


# config.h top and bottom
AH_TOP([
#ifndef CONFIG_H
#define CONFIG_H
])

AH_BOTTOM([
#if HAVE_CMATH
#  include <cmath>
#endif

/* Solaris uses <ieeefp.h> for declaring isnan() and finite() functions */
#if HAVE_IEEEFP_H
#  include <ieeefp.h>
#endif

/* Microsoft Visual C++ .NET underscore prefixed functions */
#if _MSC_VER
#  include <cfloat>
#  define HAVE_FINITE 1
#  define finite(x) _finite(x)
#  define HAVE_ISNAN 1
#  define isnan(x) _isnan(x)
#  define HAVE_FPCLASS 1
#  define fpclass(x) _fpclass(x)
#  define HAVE_JN 1
#  define jn(a, b) _jn(a, b)
#  define HAVE_YN 1
#  define yn(a, b) _yn(a, b)
#  define HAVE_J0 1
#  define j0(a) _j0(a)
#  define HAVE_J1 1
#  define j1(a) _j1(a)
#endif

#if (! defined(HAVE_ISINF) && defined(HAVE_FPCLASS))
#  define HAVE_ISINF 1
#  define isinf(a) (fpclass(a) == FP_NINF || fpclass(a) == FP_PINF)
#endif

#if (! defined (HAVE_FINITE) && defined (HAVE_ISFINITE))
#  define HAVE_FINITE 1
#  define finite(a) isfinite(a)
#endif

#if (! defined(HAVE_FINITE) && defined(HAVE_ISNAN) && defined(HAVE_ISINF))
#  define HAVE_FINITE 1
#  define finite(a) (! isnan(a) && ! isinf(a))
#endif

#endif /* #ifndef CONFIG_H */
])


# --------------------------------------------------------
# I18n (internationalisation) with GNU Gettext
# --------------------------------------------------------
#AM_GNU_GETTEXT([external], [need-formatstring-macros])
#AM_GNU_GETTEXT_VERSION([0.14])
# Force the usage of the i18n feature: on 64bit architecture,
# as gettext uses conversions from int to char*, there may be
# some issues. However, for regular translation (e.g., to German
# or French), the PO files are correctly transformed into GMO.
USE_NLS=yes


# Define configured files
AC_CONFIG_FILES(
	Makefile
	dsim-config
	dsim.m4
	dsim.pc
	dsim.spec
	stdair/Makefile
	stdair/basic/Makefile
	stdair/bom/Makefile
	stdair/factory/Makefile
	stdair/config/Makefile
	stdair/core/Makefile
	trademgen/Makefile
	trademgen/basic/Makefile
	trademgen/bom/Makefile
	trademgen/factory/Makefile
	trademgen/dbadaptor/Makefile
	trademgen/command/Makefile
	trademgen/service/Makefile
	trademgen/config/Makefile
	trademgen/core/Makefile
	trademgen/batches/Makefile
	trademgen/python/Makefile
	airsched/Makefile
	airsched/basic/Makefile
	airsched/bom/Makefile
	airsched/factory/Makefile
	airsched/command/Makefile
	airsched/service/Makefile
	airsched/config/Makefile
	airsched/core/Makefile
	airsched/batches/Makefile
	airinv/Makefile
	airinv/basic/Makefile
	airinv/bom/Makefile
	airinv/factory/Makefile
	airinv/command/Makefile
	airinv/service/Makefile
	airinv/config/Makefile
	airinv/core/Makefile
	airinv/batches/Makefile
	simcrs/Makefile
	simcrs/basic/Makefile
	simcrs/bom/Makefile
	simcrs/factory/Makefile
	simcrs/command/Makefile
	simcrs/service/Makefile
	simcrs/config/Makefile
	simcrs/core/Makefile
	simcrs/batches/Makefile
	rmol/Makefile
	rmol/basic/Makefile
	rmol/field/Makefile
	rmol/bom/Makefile
	rmol/factory/Makefile
	rmol/command/Makefile
	rmol/service/Makefile
	rmol/config/Makefile
	rmol/core/Makefile
	rmol/batches/Makefile
	dsim/Makefile
	dsim/basic/Makefile
	dsim/bom/Makefile
	dsim/factory/Makefile
	dsim/command/Makefile
	dsim/service/Makefile
	dsim/config/Makefile
	dsim/core/Makefile
	dsim/batches/Makefile
	test/Makefile
)
AC_OUTPUT

echo "
------------------------------------------------------------------------------
$PACKAGE-$VERSION library configuration:
------------------------------------------------------------------------------

Directories:
  - prefix ............ : ${prefix}
  - exec_prefix ....... : ${exec_prefix}
  - includedir ........ : ${includedir}
  - libdir ............ : ${libdir}
  - docdir ............ : ${docdir}
  - mandir ............ : ${mandir}
  - infodir ........... : ${infodir}

Switches:
  - debug ............. : ${enable_debug}
  - exceptions ........ : ${enable_exceptions}
  - info-doc .......... : ${enable_info_doc}
  - html-doc .......... : ${enable_html_doc}
  - shared ............ : ${enable_shared}
  - static ............ : ${enable_static}

Documentation tools:
  - texinfo ........... : ${texinfo_ok}
  - doxygen ........... : ${doxygen_ok}
  - latex ............. : ${latex_ok}
  - dvips ............. : ${dvips_ok}
  - ghostscript ....... : ${gs_ok}

Testing tools:
  - diff .............. : ${diff_ok}
  - sed ............... : ${sed_ok}

Compiler/linker flags/libs/defs:
  - CXX ............... : ${CXX}
  - CXXFLAGS .......... : ${CXXFLAGS_OPT}
  - CXXFLAGS_DEBUG .... : ${CXXFLAGS_DEBUG}
  - CPPFLAGS .......... : ${CPPFLAGS}
  - LDFLAGS ........... : ${LDFLAGS}
  - LIBS .............. : ${LIBS}

External libraries:
  - GSL ............... :
    o GSL_VERSION ..... : ${GSL_VERSION}
    o GSL_CFLAGS ...... : ${GSL_CFLAGS}
    o GSL_LIBS ........ : ${GSL_LIBS}

  - OPENMPI ........... :
    o OPENMPI_VERSION . : ${OPENMPI_VERSION}
    o OPENMPI_CFLAGS .. : ${OPENMPI_CFLAGS}
    o OPENMPI_LIBS .... : ${OPENMPI_LIBS}

  - MPICH2 ............ :
    o MPICH2_VERSION .. : ${MPICH2_VERSION}
    o MPICH2_CFLAGS ... : ${MPICH2_CFLAGS}
    o MPICH2_LIBS ..... : ${MPICH2_LIBS}

  - Python ........... :
    o PYTHON_VERSION . : ${PYTHON_VERSION}
    o PYTHON_CFLAGS .. : ${PYTHON_CFLAGS}
    o PYTHON_LIBS .... : ${PYTHON_LIBS}
    o PYTHON_ADD_LIBS  : ${PYTHON_ADD_LIBS}

  - Boost ............. :
    o BOOST_VERSION ... : ${BOOST_VERSION}
    o BOOST_CFLAGS .... : ${BOOST_CFLAGS}
    o BOOST_LIBS ...... : ${BOOST_LIBS}
    o BOOST_DT_LIB .... : ${BOOST_DATE_TIME_LIB}
    o BOOST_PO_LIB .... : ${BOOST_PROGRAM_OPTIONS_LIB}
    o BOOST_FS_LIB .... : ${BOOST_FILESYSTEM_LIB}
    o BOOST_THRD_LIB .. : ${BOOST_THREAD_LIB}
    o BOOST_PY_LIB .... : ${BOOST_PYTHON_LIB}
    o BOOST_ASIO_LIB .. : ${BOOST_ASIO_LIB}
    o BOOST_SIG_LIB ... : ${BOOST_SIGNALS_LIB}
    o BOOST_MPI_LIB ... : ${BOOST_MPI_LIB}
    o BOOST_MPIPY_LIB . : ${BOOST_MPI_PYTHON_LIB}

  - MySQL ............. :
    o MYSQL_version ... : ${MYSQL_VERSION}
    o MYSQL_CFLAGS .... : ${MYSQL_CFLAGS}
    o MYSQL_LIBS ...... : ${MYSQL_LIBS} 

  - MySQL++ ........... :
    o MYSQLPP_version . : ${MYSQLPP_VERSION}
    o MYSQLPP_CFLAGS .. : ${MYSQLPP_CFLAGS}
    o MYSQLPP_LIBS .... : ${MYSQLPP_LIBS}

  - SOCI .............. :	
    o SOCI_version .... : ${SOCI_VERSION}
    o SOCI_CFLAGS ..... : ${SOCI_CFLAGS}
    o SOCI_LIBS ....... : ${SOCI_LIBS}

  - XERCES ............ :
    o XERCES_VERSION .. : ${XERCES_VERSION}
    o XERCES_CFLAGS ... : ${XERCES_CFLAGS}
    o XERCES_LIBS ..... : ${XERCES_LIBS}

  - EXTRACC ........... :
    o EXTRACC_VERSION . : ${EXTRACC_VERSION}
    o EXTRACC_CFLAGS .. : ${EXTRACC_CFLAGS}
    o EXTRACC_LIBS .... : ${EXTRACC_LIBS}

  - TRAVELCCM ......... :
    o TRAVELCCM_VERSION : ${TRAVELCCM_VERSION}
    o TRAVELCCM_CFLAGS  : ${TRAVELCCM_CFLAGS}
    o TRAVELCCM_LIBS .. : ${TRAVELCCM_LIBS}

  - RMOL .............. :
    o RMOL_VERSION .... : ${RMOL_VERSION}
    o RMOL_CFLAGS ..... : ${RMOL_CFLAGS}
    o RMOL_LIBS ....... : ${RMOL_LIBS}

  - XAPIAN ............ :
    o XAPIAN_VERSION .. : ${XAPIAN_VERSION}
    o XAPIAN_CFLAGS ... : ${XAPIAN_CFLAGS}
    o XAPIAN_LIBS ..... : ${XAPIAN_LIBS}

 - CPPUNIT............ :
    o CPPUNIT_VERSION . : ${CPPUNIT_VERSION}
    o CPPUNIT_CFLAGS .. : ${CPPUNIT_CFLAGS}
    o CPPUNIT_LIBS .... : ${CPPUNIT_LIBS}

------------------------------------------------------------------------------
Now type 'make && make check' to build and test $PACKAGE-$VERSION library
------------------------------------------------------------------------------
"
