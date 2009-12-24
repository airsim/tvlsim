dnl
dnl Dumb test for Ilog library. Quick written macro, improve it if you want!
dnl So far it allows to define ILOG home (for instance: /usr/ilog) and to 
dnl use nceorilnx02 defaults if you don't provide ILOG's home.
dnl 
dnl Defines the following variables:
dnl
dnl		CFLAGS
dnl		======
dnl		ILOG_CPLEX_CFLAGS
dnl		ILOG_CONCERT_CFLAGS
dnl
dnl		STATIC LIBS
dnl		===========
dnl		ILOG_CPLEX_STATIC_LIBS
dnl		ILOG_CONCERT_STATIC_LIBS
dnl
dnl		DYNAMIC LIBS (only available for cplex, use concert in a static fashion)
dnl		============
dnl		ILOG_CPLEX_DYNAMIC_LIBS
dnl
dnl		Deprecated symbols still defined: ILOG_CFLAGS, ILOG_LDFLAGS
dnl
dnl author: daniel.perez@amadeus.com
dnl modified by: darnaud@amadeus.com
dnl

AC_DEFUN([AX_ILOG],
[
min_ilog_cplex_version=ifelse([$1], ,102,$1)
AC_MSG_CHECKING(for ILOG CPLEX - version >= $min_ilog_cplex_version)

dnl
dnl Ilog CPlex is delivered for two flavours of Linux: Debian 4 and Suse 9
dnl The Debian 4 version relies on the C library (libc) version 6, 
dnl where as the Suse 9 version relies on the libc version 5.
dnl
dnl The /proc/version file gives information on with which compiler version
dnl was compiled the kernel. GCC version <= 3.4.2 corresponds to libc version 5,
dnl where as GCC version >= 3.4.3 corresponds to libc version 6.
dnl
dnl full_gcc_full_version=`sed -e "s/\(.*\)gcc version \([0-9]\.[0-9]\.[0-9]\)\(.*\)/\2/g" /proc/version`  
full_gcc_major_version=`sed -e "s/\(.*\)gcc version \([0-9]\)\(.*\)/\2/g" /proc/version`  

dnl
dnl Check the CPU (and OS) architecture: 32 or 64 bits
dnl

if test -d /usr/lib64 ; then
   ARCHI=64
   FULL_ARCHI=x86-64
else
   ARCHI=32
   FULL_ARCHI=x86
fi

if test "x$full_gcc_major_version" = "x3"; then
   ILOG_NEEDED_ARCH="${FULL_ARCHI}_sles9.0_3.3"
else
   ILOG_NEEDED_ARCH="${FULL_ARCHI}_debian4.0_4.1"
fi

    AC_ARG_WITH( [cplex-version],
                 AC_HELP_STRING( [--with-cplex-version=VER],
                                 [Ilog CPLEX version. For instance: '121' ]
                               ),
                 [CPLEX_VERSION="$withval"],
                 [CPLEX_VERSION="121"]
               )
    AC_ARG_WITH( [concert-version],
                 AC_HELP_STRING( [--with-concert-version=VER],
                                 [Ilog CONCERT version (not needed with known CPLEX version). For instance: '29' ]
                               ),
                 [CONCERT_VERSION="$withval"],
                 [AS_CASE( [x"$CPLEX_VERSION"],
                           [x102], [CONCERT_VERSION="24"],
                           [x110], [CONCERT_VERSION="25"],
                           [x111], [CONCERT_VERSION="26"],
                           [x112], [CONCERT_VERSION="27"],
                           [x121], [CONCERT_VERSION="29"],
                          AC_MSG_ERROR( [We could not compute concert-version from cplex-version $CPLEX_VERSION. See config.log.] )
                         )] 
               )

    ILM_CPLEX_LICENSE_SIGNATURE=0
    AS_CASE( [x"$CPLEX_VERSION"],
             [x102], [
                       ILM_CPLEX_LICENSE="LICENSE amadeus-sophia-Amadeus prototypes\nRUNTIME CPLEX 10.200 NEVER UTSC52HPS6CA Y x64linux , options: m b q use=MAX , license: 852900"
                       ILM_CPLEX_LICENSE_SIGNATURE=6909115
                     ],
             [x121], [
                       ILM_CPLEX_LICENSE="LICENSE amadeus-sophia-Amadeus prototypes\nRUNTIME CPLEX 12.100 NEVER ZYVW13XMC2BJ Y x64linux , options: m b q use=MAX , license: 852900"
                       ILM_CPLEX_LICENSE_SIGNATURE=1163483
                     ],
             AC_MSG_WARN( [No ilm_CPLEX_license Runtime licence for CPLEX version $CPLEX_VERSION.] )
           )

cplex_home=/usr/ilog/cplex$CPLEX_VERSION
concert_home=/usr/ilog/concert$CONCERT_VERSION

cplex_includes="$cplex_home/include"
concert_includes="$concert_home/include"
cplex_libs="$cplex_home/bin/$ILOG_NEEDED_ARCH"
cplex_static_libs="$cplex_home/lib/$ILOG_NEEDED_ARCH/static_pic"
concert_libs="$concert_home/lib/$ILOG_NEEDED_ARCH/static_pic"

dnl By default, we do not use (rely on) Ilog CPlex

    AC_LANG([C++])
    AC_ARG_WITH([ilog],
                AC_HELP_STRING([--with-ilog=DIR],
                [Ilog home, where concert and cplex stuff goes in. For instance: '/usr/ilog' ]),
                [
                HAVE_ILOG="1"
                if test "x$withval" != "xyes"; then
                    cplex_includes="$withval/cplex$CPLEX_VERSION/include"
                    concert_includes="$withval/concert$CONCERT_VERSION/include"
                    cplex_libs="$withval/cplex$CPLEX_VERSION/bin/$ILOG_NEEDED_ARCH"
                    cplex_static_libs="$withval/cplex$CPLEX_VERSION/lib/$ILOG_NEEDED_ARCH/static_pic"
                    concert_libs="$withval/concert$CONCERT_VERSION/lib/$ILOG_NEEDED_ARCH/static_pic"
                fi
                ],
                [
                HAVE_ILOG="0"
                ])

    AC_SUBST([HAVE_ILOG])

    if test "x$HAVE_ILOG" = "x1"; then
        AC_DEFINE(HAVE_ILOG,,[define if the ILOG library is available])
        AC_DEFINE_UNQUOTED(ILM_CPLEX_LICENSE, "${ILM_CPLEX_LICENSE}", [define the value for ilm_CPLEX_license if available])
        AC_DEFINE_UNQUOTED(ILM_CPLEX_LICENSE_SIGNATURE, ${ILM_CPLEX_LICENSE_SIGNATURE}, [define the value for 	ilm_CPLEX_license_signature if available])
		CPLEX_INC="$cplex_includes"
		CONCERT_INC="$concert_includes"
		CPLEX_LIB="$cplex_libs"
		CPLEX_STATIC_LIB="$cplex_static_libs"
		CONCERT_LIB="$concert_libs"
	else
		CPLEX_INC=""
		CONCERT_INC=""
		CPLEX_LIB=""
		CONCERT_LIB=""
	fi

	if test "x$HAVE_ILOG" = "x1"; then
		dnl To be deprecated
		dnl We have removed the following: -O -fPIC -fexceptions -DNDEBUG  
		ILOG_CFLAGS="-I$CPLEX_INC -I$CONCERT_INC -DIL_STD"		
		ILOG_LDFLAGS="-L$CPLEX_LIB -L$CONCERT_LIB -lcplex$CPLEX_VERSION"		
		ILOG_RPATH="-rpath $CPLEX_LIB"
	
		dnl We have removed the following: -O -fPIC -fexceptions -DNDEBUG  
		ILOG_CPLEX_CFLAGS="-I$CPLEX_INC -DIL_STD"		
		ILOG_CPLEX_DYNAMIC_LIBS="-L$CPLEX_LIB -lcplex$CPLEX_VERSION"
		ILOG_CPLEX_STATIC_LIBS="-L$CPLEX_STATIC_LIB -lcplex"
		
		ILOG_CONCERT_CFLAGS="-I$CONCERT_INC"
		ILOG_CONCERT_STATIC_LIBS="-L$CPLEX_STATIC_LIB -L$CONCERT_LIB -lilocplex -lconcert" 

    ILOG_LD_LIBRARY_PATH="$CPLEX_LIB$PATH_SEPARATOR$CONCERT_LIB"

	else
		ILOG_CFLAGS=""
		ILOG_LDFLAGS=""
		ILOG_RPATH=""

		ILOG_CPLEX_CFLAGS=""		
		ILOG_CPLEX_DYNAMIC_LIBS=""
		ILOG_CPLEX_STATIC_LIBS=""
		
		ILOG_CONCERT_CFLAGS=""
		ILOG_CONCERT_STATIC_LIBS="" 
    ILOG_LD_LIBRARY_PATH=""
  fi

	dnl Cplex dynamic library location
	AC_SUBST([CPLEX_LIB])
	AC_SUBST([CPLEX_INC])

	AC_SUBST([ILOG_CFLAGS])
	AC_SUBST([ILOG_LDFLAGS])
	AC_SUBST([ILOG_RPATH])
	AC_SUBST([ILOG_CPLEX_CFLAGS])
	AC_SUBST([ILOG_CPLEX_DYNAMIC_LIBS])
	AC_SUBST([ILOG_CPLEX_STATIC_LIBS])
	AC_SUBST([ILOG_CONCERT_CFLAGS])
	AC_SUBST([ILOG_CONCERT_STATIC_LIBS])
	AC_SUBST([ILOG_LD_LIBRARY_PATH])
	

	dnl Check for successfull compilation of Ilog CPlex only when required
	if test "x$HAVE_ILOG" = "x1"; then

		SAVED_CPPFLAGS="$CPPFLAGS"
		SAVED_LDFLAGS="$LDFLAGS"
		CPPFLAGS="$CPPFLAGS $ILOG_CFLAGS"
		LDFLAGS="$LDFLAGS $ILOG_LDFLAGS"


     	AC_COMPILE_IFELSE(
				AC_LANG_PROGRAM([[
				#include <ilcplex/ilocplex.h> 
				]],
				[[int i=0;]]
				)
				,
				[
dnl		AC_MSG_RESULT([ILOG software found & working! :
dnl		
dnl	- ILOG_CPLEX_CFLAGS    		= "$ILOG_CPLEX_CFLAGS"
dnl	- ILOG_CPLEX_DYNAMIC_LIBS  	= "$ILOG_CPLEX_DYNAMIC_LIBS"
dnl	- ILOG_CPLEX_STATIC_LIBS    	= "$ILOG_CPLEX_STATIC_LIBS"
dnl	- ILOG_CONCERT_CFLAGS  		= "$ILOG_CONCERT_CFLAGS"
dnl	- ILOG_CONCERT_STATIC_LIBS  	= "$ILOG_CONCERT_STATIC_LIBS"
dnl				])
          AC_MSG_RESULT([found])    
				],
				[
		AC_MSG_ERROR([We could not compile a simple CPLEX example. See config.log.])
				]	
		)

		CPPFLAGS="$SAVED_CPPFLAGS"
		LDFLAGS="$SAVED_LDFLAGS"

	fi
])

