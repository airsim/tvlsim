## test/travelccm sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
nofail_PRG = TravelChoiceTestSuite
fail_PRG =

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

#
TravelChoiceTestSuite_SOURCES = TravelChoiceTestSuite.cpp
TravelChoiceTestSuite_CXXFLAGS= $(BOOST_CFLAGS)  \
	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK $(STDAIR_CFLAGS)
TravelChoiceTestSuite_LDADD = 
TravelChoiceTestSuite_LDFLAGS = $(BOOST_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/travelccm/core/libtravelccm.la
