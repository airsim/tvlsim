## test/dsim sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = SimulationTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

SimulationTestSuite_SOURCES = SimulationTestSuite.hpp SimulationTestSuite.cpp
SimulationTestSuite_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
SimulationTestSuite_LDADD =
SimulationTestSuite_LDFLAGS = \
		$(BOOST_LIBS) $(CPPUNIT_LIBS) \
		$(top_builddir)/extracppunit/libextracppunit.la \
		$(top_builddir)/stdair/core/libstdair.la \
		$(top_builddir)/dsim/core/libdsim.la

#simulate_SOURCES = simulate.cpp
#simulate_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
#simulate_LDADD =
#simulate_LDFLAGS = $(BOOST_LIBS) \
#		$(top_builddir)/stdair/core/libstdair.la \
#		$(top_builddir)/dsim/core/libdsim.la

