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
SimulationTestSuite_CXXFLAGS = $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
SimulationTestSuite_LDADD =
SimulationTestSuite_LDFLAGS = \
	$(BOOST_LIBS) $(CPPUNIT_LIBS) $(EXTRACC_LIBS) $(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la \
	$(top_builddir)/dsim/core/libdsim.la

