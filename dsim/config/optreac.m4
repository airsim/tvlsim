#
# Configure path for the OPTREAC library.
# Denis Arnaud <denis_arnaud@users.sourceforge.net>, July 2008
#
# Note: as the OPTREAC library depends upon BOOST to build.
# Your configure.ac must define appropriately the BOOST_CFLAGS variables.
#


AC_DEFUN([AM_PATH_OPTREAC],
[
AC_LANG_SAVE
AC_LANG([C++])

dnl By default, we do not use (rely on) the Optimisation Re-accommodation
dnl (OptReac) library.
HAVE_OPTREAC="0"

AC_ARG_WITH(optreac,
	[ --with-optreac=PFX   Prefix where OPTREAC is installed (optional)],
    optreac_dir="$withval",
	optreac_dir="")

  if test "x${OPTREAC_CONFIG+set}" != xset ; then
     if test "x$optreac_dir" != x ; then
         OPTREAC_CONFIG="$optreac_dir/bin/optreac-config"
     fi
  fi

  AC_PATH_PROG(OPTREAC_CONFIG, optreac-config, no)

  # Check whether Boost flags and libraries are defined
  AC_MSG_CHECKING(for BOOST_CFLAGS environment variable)
  if test x"${BOOST_CFLAGS}" = x; then
	AC_MSG_RESULT([Warning: OPTREAC needs Boost, and the BOOST_CFLAGS environment variable does not appear to be set. It may not be a problem, though, if your Unix distribution is standard, that is, if Boost is installed in /usr/local. Otherwise, the OPTREAC will fail to compile.])
	else
	AC_MSG_RESULT([ok (set to ${BOOST_CFLAGS})])
  fi

  AC_MSG_CHECKING(for BOOST_DATE_TIME_LIB environment variable)
  if test x"${BOOST_DATE_TIME_LIB}" = x; then
	AC_MSG_RESULT([Warning: OPTREAC needs Boost Date-Time library, and the BOOST_DATE_TIME_LIB environment variable does not appears to be set. The OPTREAC may fail to link.])
	else
	AC_MSG_RESULT([ok (set to ${BOOST_DATE_TIME_LIB})])
  fi

  min_optreac_version=ifelse([$1], ,0.11.0,$1)
  AC_MSG_CHECKING(for OPTREAC - version >= $min_optreac_version)
  no_optreac=""
  if test "${OPTREAC_CONFIG}" = "no" ; then
    no_optreac=yes
	AC_MSG_RESULT([no])
	AC_SUBST([HAVE_OPTREAC])
  else
	OPTREAC_VERSION=`${OPTREAC_CONFIG} --version`
    OPTREAC_CFLAGS=`${OPTREAC_CONFIG} --cflags`
    OPTREAC_CFLAGS="${BOOST_CFLAGS} ${OPTREAC_CFLAGS}"
    OPTREAC_LIBS=`${OPTREAC_CONFIG} --libs`
    OPTREAC_LIBS="${BOOST_LIBS} ${BOOST_DATE_TIME_LIB} ${OPTREAC_LIBS}"

	HAVE_OPTREAC="1"
    AC_SUBST([HAVE_OPTREAC])
	AC_SUBST([OPTREAC_VERSION])
	AC_SUBST([OPTREAC_CFLAGS])
	AC_SUBST([OPTREAC_LIBS])

    optreac_major_version=`echo ${OPTREAC_VERSION} | sed 's/^\([[0-9]]*\).*/\1/'`
    if test "x${optreac_major_version}" = "x" ; then
       optreac_major_version=0
    fi

    optreac_minor_version=`echo ${OPTREAC_VERSION} | \
						sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`
    if test "x${optreac_minor_version}" = "x" ; then
       optreac_minor_version=0
    fi

    optreac_micro_version=`echo ${OPTREAC_VERSION} | \
          sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`
    if test "x${optreac_micro_version}" = "x" ; then
       optreac_micro_version=0
    fi


	SAVED_CPPFLAGS="${CPPFLAGS}"
	SAVED_LDFLAGS="${LDFLAGS}"
	CPPFLAGS="${CPPFLAGS} ${BOOST_CFLAGS} ${OPTREAC_CFLAGS}"
	LDFLAGS="${LDFLAGS} ${OPTREAC_LIBS}"


   	AC_COMPILE_IFELSE(
		AC_LANG_PROGRAM([[
				#include <optreac/OPTREAC_Service.hpp> 
				]],
				[[int i=0;]]
		)
		,

    	[AC_MSG_RESULT([yes (${OPTREAC_VERSION})])],

		[
		AC_MSG_ERROR([We could not compile a simple OPTREAC example. See config.log.])
		]
	)

	CPPFLAGS="${SAVED_CPPFLAGS}"
	LDFLAGS="${SAVED_LDFLAGS}"

  fi

AC_LANG_RESTORE
])
