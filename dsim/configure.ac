# Process this file with autoconf to produce a configure script
#-------------------------------------------------------------------
AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (C) 2007-2010 Denis Arnaud <denis_arnaud@users.sourceforge.net>])
AC_INIT([DSIM],[99.99.99],[denis_arnaud@users.sourceforge.net],[dsim])
AC_CONFIG_HEADER([dsim/config.h])
AC_CONFIG_SRCDIR([dsim])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])
AM_INIT_AUTOMAKE
AM_PATH_CPPUNIT(1.10)
AM_CONDITIONAL([HAVE_CPPUNIT], [test "x$CPPUNIT_LIBS" != x])

# Shared library versioning for the main library
GENERIC_LIBRARY_VERSION="99:99:99"
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


# Default compilation flags
if test -z "${CFLAGS}"; then
   CFLAGS="-g -Wall"
fi

if test -z "${CXXFLAGS}"; then
   CXXFLAGS="-g -Wall"
fi


# -----------------------------------------------------------------------------
# Support for ExtraCC (Extra-CruiseControl): http://sf.net/projects/extracc
# -----------------------------------------------------------------------------
# Note: the ExtraCC is now imported as a Subversion external reference,
#       thus directly accessible within the extracppunit directory.
#       When that library will be widely avaible on Linux distributions,
#       the external reference can be removed and the following line uncommented.
AM_PATH_EXTRACC
AC_SUBST(EXTRACC_VERSION)
AC_SUBST(EXTRACC_CFLAGS)
AC_SUBST(EXTRACC_LIBS)

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
# OpenMPI: http://www.open-mpi.org
# -----------------------------------------------------------
AX_OPENMPI
AC_SUBST(MPIGEN_VERSION)
AC_SUBST(MPIGEN_CFLAGS)
AC_SUBST(MPIGEN_LIBS)

# -----------------------------------------------------------
# MPICH2: http://www.mcs.anl.gov/research/projects/mpich2
# Note: Boost.MPI depends on MPICH2 by default, rather than 
#       on OpenMPI
# -----------------------------------------------------------
#AX_MPICH2
#AC_SUBST(MPIGEN_VERSION)
#AC_SUBST(MPIGEN_CFLAGS)
#AC_SUBST(MPIGEN_LIBS)

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
AC_SUBST(BOOST_IOSTREAMS_LIB)
AC_SUBST(BOOST_ASIO_LIB)
AC_SUBST(BOOST_MPI_LIB)
AC_SUBST(BOOST_MPI_PYTHON_LIB)

# --------------------------------------------------------------------
# Support for MySQL (C client API): http://www.mysql.org
# --------------------------------------------------------------------
AX_MYSQL
AC_SUBST(MYSQL_VERSION)
AC_SUBST(MYSQL_CFLAGS)
AC_SUBST(MYSQL_LIBS)

# --------------------------------------------------------------------
# Support for SOCI http://soci.sourceforge.net
# --------------------------------------------------------------------
AX_SOCI
AC_SUBST(SOCI_VERSION)
AC_SUBST(SOCI_CFLAGS)
AC_SUBST(SOCI_LIBS)

# -----------------------------------------------------------------------------
# Support for StdAir (): http://sf.net/projects/stdair
# -----------------------------------------------------------------------------
AM_PATH_STDAIR
AC_SUBST(STDAIR_VERSION)
AC_SUBST(STDAIR_CFLAGS)
AC_SUBST(STDAIR_LIBS)

# --------------------------------------------------------------------
# Support for TRAVEL-CCM (CCM Open Library): http://sf.net/projects/travel-ccm
# --------------------------------------------------------------------
# Note: That library is imported as a Subversion external reference. So, there is
#       no need to import it as an independent library as well.
#AM_PATH_TRAVELCCM
#AC_SUBST(TRAVELCCM_VERSION)
#AC_SUBST(TRAVELCCM_CFLAGS)
#AC_SUBST(TRAVELCCM_LIBS)

# --------------------------------------------------------------------
# Support for RMOL (RM Open Library): http://sf.net/projects/rmol
# --------------------------------------------------------------------
# Note: That library is imported as a Subversion external reference. So, there is
#       no need to import it as an independent library as well.
#AM_PATH_RMOL([0.19])
#AC_SUBST(RMOL_VERSION)
#AC_SUBST(RMOL_CFLAGS)
#AC_SUBST(RMOL_LIBS)

# ============ X E R C E S ==================
AX_XERCES_C

#
# librt
#
AC_CHECK_LIBRT

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
	airrac/Makefile
	airrac/basic/Makefile
	airrac/bom/Makefile
	airrac/factory/Makefile
	airrac/command/Makefile
	airrac/service/Makefile
	airrac/config/Makefile
	airrac/core/Makefile
	airrac/batches/Makefile
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
	airinv/Makefile
	airinv/basic/Makefile
	airinv/bom/Makefile
	airinv/factory/Makefile
	airinv/command/Makefile
	airinv/service/Makefile
	airinv/config/Makefile
	airinv/core/Makefile
	airinv/batches/Makefile
	airinv/server/Makefile
	avlcal/Makefile
	avlcal/basic/Makefile
	avlcal/bom/Makefile
	avlcal/factory/Makefile
	avlcal/command/Makefile
	avlcal/service/Makefile
	avlcal/config/Makefile
	avlcal/core/Makefile
	avlcal/batches/Makefile
	simfqt/Makefile
	simfqt/basic/Makefile
	simfqt/bom/Makefile
	simfqt/factory/Makefile
	simfqt/command/Makefile
	simfqt/service/Makefile
	simfqt/config/Makefile
	simfqt/core/Makefile
	simfqt/batches/Makefile
	simcrs/Makefile
	simcrs/basic/Makefile
	simcrs/bom/Makefile
	simcrs/factory/Makefile
	simcrs/command/Makefile
	simcrs/service/Makefile
	simcrs/config/Makefile
	simcrs/core/Makefile
	simcrs/batches/Makefile
	travelccm/Makefile
	travelccm/basic/Makefile
	travelccm/bom/Makefile
	travelccm/factory/Makefile
	travelccm/command/Makefile
	travelccm/service/Makefile
	travelccm/core/Makefile
	travelccm/config/Makefile
	travelccm/batches/Makefile
	dsim/Makefile
	dsim/basic/Makefile
	dsim/bom/Makefile
	dsim/factory/Makefile
	dsim/command/Makefile
	dsim/service/Makefile
	dsim/config/Makefile
	dsim/core/Makefile
	dsim/batches/Makefile
	doc/Makefile
	doc/images/Makefile
	doc/tutorial/Makefile
	doc/tutorial/src/Makefile
	doc/local/Makefile
	doc/local/index.doc
	doc/doxygen_html.cfg
	doc/sourceforge/howto_release_dsim.html
	test/Makefile
	test/samples/Makefile
	test/boost/Makefile
	test/boost/accumulators/Makefile
	test/boost/asio/Makefile
	test/boost/asio/httpd/Makefile
	test/boost/asio/logd/Makefile
	test/boost/asio/log_server/Makefile
	test/boost/asio/logger/Makefile
	test/boost/asio/chat/Makefile
	test/boost/filesystem/Makefile
	test/boost/iostreams/Makefile
	test/boost/mpi/Makefile
	test/boost/mpi/master_slave/Makefile
	test/boost/mpi/log_server/Makefile
	test/boost/mpl/Makefile
	test/boost/spirit/Makefile
	test/boost/serialization/Makefile
	test/boost/signals/Makefile
	test/boost/smart_pointers/Makefile
	test/trademgen/Makefile
	test/airsched/Makefile
	test/airrac/Makefile
	test/rmol/Makefile
	test/airinv/Makefile
	test/avlcal/Makefile
	test/simfqt/Makefile
	test/simcrs/Makefile
	test/travelccm/Makefile
	test/dsim/Makefile
	test/master_slave/Makefile
)
AC_OUTPUT

# OpenMPI
if test "x$HAVE_OPENMPI" = "x1"; then
   OPENMPI_OUTPUT="  - OPENMPI ........... :
    o OPENMPI_VERSION . : ${MPIGEN_VERSION}
    o OPENMPI_CFLAGS .. : ${MPIGEN_CFLAGS}
    o OPENMPI_LIBS .... : ${MPIGEN_LIBS}"
else
   OPENMPI_OUTPUT="  - OPENMPI ........... : No"
fi

# MPICH2
if test "x$HAVE_MPICH2" = "x1"; then
   MPICH2_OUTPUT="  - MPICH2 ............ :
    o MPICH2_VERSION .. : ${MPIGEN_VERSION}
    o MPICH2_CFLAGS ... : ${MPIGEN_CFLAGS}
    o MPICH2_LIBS ..... : ${MPIGEN_LIBS}"
else
   MPICH2_OUTPUT="  - MPICH2 ............ : No"
fi

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

${OPENMPI_OUTPUT}

${MPICH2_OUTPUT}

  - Python ............ :
    o PYTHON_VERSION .. : ${PYTHON_VERSION}
    o PYTHON_CFLAGS ... : ${PYTHON_CFLAGS}
    o PYTHON_LIBS ..... : ${PYTHON_LIBS}
    o PYTHON_ADD_LIBS . : ${PYTHON_ADD_LIBS}

  - Boost ............. :
    o BOOST_VERSION ... : ${BOOST_VERSION}
    o BOOST_CFLAGS .... : ${BOOST_CFLAGS}
    o BOOST_LIBS ...... : ${BOOST_LIBS}
    o BOOST_DT_LIB .... : ${BOOST_DATE_TIME_LIB}
    o BOOST_PO_LIB .... : ${BOOST_PROGRAM_OPTIONS_LIB}
    o BOOST_FS_LIB .... : ${BOOST_FILESYSTEM_LIB}
    o BOOST_THRD_LIB .. : ${BOOST_THREAD_LIB}
    o BOOST_PY_LIB .... : ${BOOST_PYTHON_LIB}
    o BOOST_IOSTR_LIB . : ${BOOST_IOSTREAMS_LIB}
    o BOOST_ASIO_LIB .. : ${BOOST_ASIO_LIB}
    o BOOST_SIG_LIB ... : ${BOOST_SIGNALS_LIB}
    o BOOST_MPI_LIB ... : ${BOOST_MPI_LIB}
    o BOOST_MPIPY_LIB . : ${BOOST_MPI_PYTHON_LIB}

  - MySQL ............. :
    o MYSQL_version ... : ${MYSQL_VERSION}
    o MYSQL_CFLAGS .... : ${MYSQL_CFLAGS}
    o MYSQL_LIBS ...... : ${MYSQL_LIBS} 

  - SOCI .............. :	
    o SOCI_version .... : ${SOCI_VERSION}
    o SOCI_CFLAGS ..... : ${SOCI_CFLAGS}
    o SOCI_LIBS ....... : ${SOCI_LIBS}

  - XERCES ............ :
    o XERCES_VERSION .. : ${XERCES_VERSION}
    o XERCES_CFLAGS ... : ${XERCES_CFLAGS}
    o XERCES_LIBS ..... : ${XERCES_LIBS}

  - StdAir ........... :
    o STDAIR_VERSION .. : ${STDAIR_VERSION}
    o STDAIR_CFLAGS ... : ${STDAIR_CFLAGS}
    o STDAIR_LIBS ..... : ${STDAIR_LIBS}

  - ExtraCC ........... :
    o EXTRACC_VERSION . : ${EXTRACC_VERSION}
    o EXTRACC_CFLAGS .. : ${EXTRACC_CFLAGS}
    o EXTRACC_LIBS .... : ${EXTRACC_LIBS}

 - CPPUNIT............ :
    o CPPUNIT_VERSION . : ${CPPUNIT_VERSION}
    o CPPUNIT_CFLAGS .. : ${CPPUNIT_CFLAGS}
    o CPPUNIT_LIBS .... : ${CPPUNIT_LIBS}

------------------------------------------------------------------------------
Now type 'make && make check' to build and test $PACKAGE-$VERSION library
------------------------------------------------------------------------------
"
