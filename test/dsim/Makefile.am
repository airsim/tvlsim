## test/dsim sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
nofail_PRG = SimulationTestSuite
fail_PRG =

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

SimulationTestSuite_SOURCES = SimulationTestSuite.cpp
SimulationTestSuite_CXXFLAGS = $(BOOST_CFLAGS) \
	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK \
	$(STDAIR_CFLAGS)
SimulationTestSuite_LDADD =
SimulationTestSuite_LDFLAGS = $(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB) \
	$(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la \
	$(top_builddir)/dsim/core/libdsim.la

