## test/simcrs sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
nofail_PRG = CRSTestSuite
fail_PRG =

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

#
CRSTestSuite_SOURCES = CRSTestSuite.cpp
CRSTestSuite_CXXFLAGS = $(BOOST_CFLAGS)  \
	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK $(STDAIR_CFLAGS)
CRSTestSuite_LDADD =
CRSTestSuite_LDFLAGS = \
		$(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(STDAIR_LIBS) \
		$(top_builddir)/simfqt/core/libsimfqt.la \
		$(top_builddir)/simcrs/core/libsimcrs.la
#		$(top_builddir)/airinv/core/libairinv.la
