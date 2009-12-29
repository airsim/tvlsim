## test sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS = boost airsched rmol

##
check_PROGRAMS = simulate
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

#SimulationTestSuite_SOURCES = SimulationTestSuite.hpp \
				SimulationTestSuite.cpp
#SimulationTestSuite_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
#SimulationTestSuite_LDADD = $(top_builddir)/test/com/libcppunitcore.la
#SimulationTestSuite_LDFLAGS = \
#		$(BOOST_LIBS) $(CPPUNIT_LIBS) \
#		$(top_builddir)/dsim/libdsim.la

simulate_SOURCES = simulate.cpp
simulate_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
simulate_LDADD =
simulate_LDFLAGS = $(BOOST_LIBS) \
		$(top_builddir)/dsim/core/libdsim.la

EXTRA_DIST =
