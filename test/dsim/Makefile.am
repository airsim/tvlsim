## test/dsim sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
SUBDIRS =

EXTRA_DIST =

##
nofail_PRG = SimulationTestSuite
fail_PRG =

#
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${nofail_PRG:%=%_utfresults.xml} ${nofail_PRG:%=%.log}

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

#
SimulationTestSuite_SOURCES = $(dsim_test_bin_cc_sources)
SimulationTestSuite_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
SimulationTestSuite_LDADD =
SimulationTestSuite_LDFLAGS = $(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB) \
	$(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la \
	$(top_builddir)/dsim/core/libdsim.la

