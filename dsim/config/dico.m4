#
# Configure path for the DICO library.
# Denis Arnaud <denis_arnaud@users.sourceforge.net>, July 2008
#
# Note: as the DICO library depends upon BOOST to build.
# Your configure.ac must define appropriately the BOOST_CFLAGS variables.
#


AC_DEFUN([AM_PATH_DICO],
[
AC_LANG_SAVE
AC_LANG([C++])

dnl By default, we do not use (rely on) the Dynamic Inventory Controls
dnl (DICO) library.
HAVE_DICO="0"

AC_ARG_WITH(dico,
	[ --with-dico=PFX   Prefix where DICO is installed (optional)],
    dico_dir="$withval",
	dico_dir="")

  if test "x${DICO_CONFIG+set}" != xset ; then
     if test "x$dico_dir" != x ; then
         DICO_CONFIG="$dico_dir/bin/dico-config"
     fi
  fi

  AC_PATH_PROG(DICO_CONFIG, dico-config, no)

  # Check whether Boost flags and libraries are defined
  AC_MSG_CHECKING(for BOOST_CFLAGS environment variable)
  if test x"${BOOST_CFLAGS}" = x; then
	AC_MSG_RESULT([Warning: DICO needs Boost, and the BOOST_CFLAGS environment variable does not appear to be set. It may not be a problem, though, if your Unix distribution is standard, that is, if Boost is installed in /usr/local. Otherwise, the DICO will fail to compile.])
	else
	AC_MSG_RESULT([ok (set to ${BOOST_CFLAGS})])
  fi

  AC_MSG_CHECKING(for BOOST_DATE_TIME_LIB environment variable)
  if test x"${BOOST_DATE_TIME_LIB}" = x; then
	AC_MSG_RESULT([Warning: DICO needs Boost Date-Time library, and the BOOST_DATE_TIME_LIB environment variable does not appears to be set. The DICO may fail to link.])
	else
	AC_MSG_RESULT([ok (set to ${BOOST_DATE_TIME_LIB})])
  fi

  min_dico_version=ifelse([$1], ,0.11.0,$1)
  AC_MSG_CHECKING(for DICO - version >= $min_dico_version)
  no_dico=""
  if test "${DICO_CONFIG}" = "no" ; then
    no_dico=yes
	AC_MSG_RESULT([no])
	AC_SUBST([HAVE_DICO])
  else
	DICO_VERSION=`${DICO_CONFIG} --version`
    DICO_CFLAGS="${BOOST_CFLAGS} `${DICO_CONFIG} --cflags` -DHAVE_DICO"
    DICO_LIBS="${BOOST_DATE_TIME_LIB} `${DICO_CONFIG} --libs`"
    DICO_ILOG_RPATH="`${DICO_CONFIG} --ilog-rpath`"

	HAVE_DICO="1"
    AC_SUBST([HAVE_DICO])
	AC_SUBST([DICO_VERSION])
	AC_SUBST([DICO_CFLAGS])
	AC_SUBST([DICO_LIBS])
	AC_SUBST([DICO_ILOG_RPATH])

    dico_major_version=`echo ${DICO_VERSION} | sed 's/^\([[0-9]]*\).*/\1/'`
    if test "x${dico_major_version}" = "x" ; then
       dico_major_version=0
    fi

    dico_minor_version=`echo ${DICO_VERSION} | \
						sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`
    if test "x${dico_minor_version}" = "x" ; then
       dico_minor_version=0
    fi

    dico_micro_version=`echo ${DICO_VERSION} | \
          sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`
    if test "x${dico_micro_version}" = "x" ; then
       dico_micro_version=0
    fi


	SAVED_CPPFLAGS="${CPPFLAGS}"
	SAVED_LDFLAGS="${LDFLAGS}"
	CPPFLAGS="${CPPFLAGS} ${BOOST_CFLAGS} ${DICO_CFLAGS}"
	LDFLAGS="${LDFLAGS} ${DICO_LIBS}"


   	AC_COMPILE_IFELSE(
		AC_LANG_PROGRAM([[
				#include <dico/DICO_Service.hpp> 
				]],
				[[int i=0;]]
		)
		,

    	[AC_MSG_RESULT([yes (${DICO_VERSION})])],

		[
		AC_MSG_ERROR([We could not compile a simple DICO example. See config.log.])
		]
	)

	CPPFLAGS="${SAVED_CPPFLAGS}"
	LDFLAGS="${SAVED_LDFLAGS}"

  fi

AC_LANG_RESTORE
])

