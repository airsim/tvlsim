## test/simlfs sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = LFSTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

LFSTestSuite_SOURCES = LFSTestSuite.hpp LFSTestSuite.cpp
LFSTestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
LFSTestSuite_LDADD =
LFSTestSuite_LDFLAGS = $(CPPUNIT_LIBS) $(EXTRACC_LIBS) $(STDAIR_LIBS) \
	$(top_builddir)/simlfs/core/libsimlfs.la
