## test/airsched sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = AirlineScheduleTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

AirlineScheduleTestSuite_SOURCES = AirlineScheduleTestSuite.hpp \
								AirlineScheduleTestSuite.cpp
AirlineScheduleTestSuite_CXXFLAGS= $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
AirlineScheduleTestSuite_LDADD = 
AirlineScheduleTestSuite_LDFLAGS = $(BOOST_LIBS) $(CPPUNIT_LIBS) \
	$(EXTRACC_LIBS) $(STDAIR_LIBS) \
	$(top_builddir)/airsched/core/libairsched.la

