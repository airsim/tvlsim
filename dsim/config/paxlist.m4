#
# Configure path for the PAXLIST library.
# Denis Arnaud <denis_arnaud@users.sourceforge.net>, July 2008
#
# Note: as the PAXLIST library depends upon BOOST to build,
# your configure.ac must define appropriately the BOOST_CFLAGS
# variable.
#
HAVE_PAXLIST=0
AC_DEFUN([AM_PATH_PAXLIST],
[
	AC_MSG_CHECKING([for PaxList])
	AC_ARG_WITH(paxlist,
		AS_HELP_STRING([--with-paxlist],
	              [Prefix where paxlist is installed]),
	    [],
		[])
		
	AS_IF([test -d "$with_paxlist"],
		[
			HAVE_PAXLIST=1
			AC_DEFINE(HAVE_PAXLIST,1,[if PaxList was found])

			AC_ARG_WITH(paxlist,
			AS_HELP_STRING([--with-paxlist-type],
				[Type of the paxlist adaptor to use ([edi(default)|file])]),
				[],
				[with_paxlist_type="edi"])

			AS_IF([test "x$with_paxlist_type" = xedi],
				[
				AC_SUBST(AMD_ROOT_DIR,"/projects/mwdeldev")
				AC_SUBST(CFC_VERSION,"2-8-0-45")
				AC_SUBST(IDSC_VERSION,"1-12-6-0")
				AC_SUBST(AMDTB_VERSION,"2-8-2-0")
				AC_SUBST(UP_DATE_VERSION,"2-16-2-0")
				AC_SUBST(EDICPP_VERSION,"4-0-0-5")
				AC_SUBST(UP_TCP_VERSION,"3-7-1-2")
				AC_SUBST(AMD_ENV,"64")

				AC_SUBST(__DUMMY_VARIABLE_TO_ADD_PAXLIST_CODE__,
					['
					comp_vers Linux 4.3.3-8) = 3_4_2
				include $(AMD_ROOT_DIR)/CFC/$(CFC_VERSION)/etc/Makefile.inc
				PROCESSOR_TYPE := $(if $(filter unknown,$(PROCESSOR_TYPE)),$(shell uname -m),$(PROCESSOR_TYPE))
				UP_TCP_LIGHT = 1
				'])

			AC_SUBST(PAXLIST_CFLAGS,["-I$with_paxlist/include"])

			AC_SUBST(PAXLIST_LIBS,["$with_paxlist/lib/libpaxlist.la \
					$with_paxlist/lib/libpaxlistediadaptor.la \
					"'$(UP_TCP_LIBS) $(UP_TCP_CLIBS) $(IDSC_LIBS) $(EDICPP_LIBS) $(UP_DATE_LIBS) $(AMDTB_LIBS) $(RT_LIBS)'])
			],
			[
			AC_SUBST(PAXLIST_CFLAGS,["-I$with_paxlist/include"])

			AC_SUBST(PAXLIST_LIBS,["$with_paxlist/lib/libpaxlist.la \
					$with_paxlist/lib/libpaxlistfileadaptor.la"])
			])
		]
		,
		[
			dnl AC_MSG_FAILURE([--with-paxlist is mandatory and must be a valid directory])
			HAVE_PAXLIST=0
			AC_SUBST([HAVE_PAXLIST])
			AC_DEFINE(HAVE_PAXLIST,0,[if PaxList was found])

       echo " "
       echo " "
       echo "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ "
       echo "  Warning:                                                   "
       echo "                                                             "
       echo "   --with-paxlist was not supplied. Some parts of this       "
       echo "   application rely on it and won't be compiled.             "
       echo "                                                             "
       echo "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ "
       echo " "

		])
			
	AM_CONDITIONAL(HAVE_PAXLIST,test "x$HAVE_PAXLIST" = "x1" )

])
