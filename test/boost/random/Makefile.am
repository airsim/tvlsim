## test/boost/random sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST =

##
# Test library
noinst_LTLIBRARIES = librndmgr.la

librndmgr_la_SOURCES = $(librndmgr_h_sources) $(librndmgr_cc_sources)
librndmgr_la_CXXFLAGS = $(BOOST_CFLAGS)
librndmgr_la_LIBADD = 
librndmgr_la_LDFLAGS =

#
nofail_PRG = SimulateTestSuite
extra_PRG = random_demo random_order_stats
fail_PRG =

#
check_PROGRAMS = $(nofail_PRG) $(extra_PRG) $(fail_PRG)
TESTS = $(nofail_PRG) $(fail_PRG)
XFAIL_TESTS = $(fail_PRG)

#
random_demo_SOURCES = random_demo.cpp
random_demo_CXXFLAGS = $(BOOST_CFLAGS)
random_demo_LDFLAGS = $(BOOST_LIBS)

random_order_stats_SOURCES = random_order_stats.cpp
random_order_stats_CXXFLAGS = $(BOOST_CFLAGS)
random_order_stats_LDFLAGS = $(BOOST_LIBS) $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(builddir)/librndmgr.la

SimulateTestSuite_SOURCES = SimulateTestSuite.cpp
SimulateTestSuite_CXXFLAGS = $(BOOST_CFLAGS) \
	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK
SimulateTestSuite_LDADD =
SimulateTestSuite_LDFLAGS = $(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)
