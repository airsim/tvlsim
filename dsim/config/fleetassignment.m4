dnl
dnl Test for the fleetassignment library.
dnl
dnl author: christophe-marie.duquesne@amadeus.com
dnl
dnl This macro enables the "--with-fleetassignment=" user switch.
dnl If the user does not use the switch, it tries to find the fleetassignment 
dnl header in the default path. It stops the configure process if not found.
dnl
dnl - it substitutes the following values for your makefiles
dnl FLEETASSIGNMENT_CFLAGS
dnl FLEETASSIGNMENT_LDFLAGS
dnl
dnl - it defines HAVE_FLEETASSIGNMENT for your config.h
dnl

AC_DEFUN(
    [AX_FLEETASSIGNMENT],
    [
    dnl we save the compiler flags and declare the language
    save_CFLAGS=$CFLAGS
    save_CPPFLAGS=$CFLAGS
    AC_LANG([C++])

    dnl we add the user switch
    AC_ARG_WITH(
        fleetassignment,
        AC_HELP_STRING(
            [--with-fleetassignment=DIR],
            [fleetassignment home (if installed in a non-standard dir)]),
        [fleetassignment_home="$withval"],
        [fleetassignment_home=]
        )

    AC_MSG_CHECKING([for the fleetassignment library])

    dnl we set the compiler flags accordingly
    if test "x${fleetassignment_home}" = "xno" \
         -o "x${fleetassignment_home}" = "x"
    then
        FLEETASSIGNMENT_CFLAGS=
        FLEETASSIGNMENT_LDFLAGS=
    else
        FLEETASSIGNMENT_CFLAGS="-I${fleetassignment_home}/include"
        FLEETASSIGNMENT_LDFLAGS="-L${fleetassignment_home}/lib/"
    fi
    CFLAGS="$FLEETASSIGNMENT_CFLAGS"
    CPPFLAGS="$FLEETASSIGNMENT_CFLAGS"

    dnl then we look for the headers (this uses the previous flags)
    AC_CHECK_HEADER(
        [fleetassignment/fleetassignment.hpp],
        [],
        [
        AC_MSG_ERROR(
            [Could not find fleetassignment header]
            )
        ]
    )

    dnl The previous step suceeded : we can do all the substitutions
    HAVE_FLEETASSIGNMENT=1	
    AM_CONDITIONAL(
        [HAVE_FLEETASSIGNMENT],
        [test "x${HAVE_FLEETASSIGNMENT}" = "x1"]
        )
    AC_DEFINE(
        [HAVE_FLEETASSIGNMENT],
        [],
        [Define this if fleetassignment is installed]
        )
    AC_SUBST([FLEETASSIGNMENT_CFLAGS])
    AC_SUBST([FLEETASSIGNMENT_LDFLAGS])
    AC_MSG_RESULT([using flags ${FLEETASSIGNMENT_LDFLAGS}])

    dnl we restore the compiler flags
    CFLAGS=$save_CFLAGS
    CPPFLAGS=$save_CFLAGS
    ]
    )
