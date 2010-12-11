## test/simcrs sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = CRSTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

CRSTestSuite_SOURCES = CRSTestSuite.hpp CRSTestSuite.cpp
CRSTestSuite_CXXFLAGS = $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
CRSTestSuite_LDADD =
CRSTestSuite_LDFLAGS = \
		$(BOOST_LIBS) $(CPPUNIT_LIBS) $(EXTRACC_LIBS) $(STDAIR_LIBS) \
		$(top_builddir)/simfqt/core/libsimfqt.la \
		$(top_builddir)/simcrs/core/libsimcrs.la
#		$(top_builddir)/airinv/core/libairinv.la
