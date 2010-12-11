## test/avlcal sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = AvlCalTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

AvlCalTestSuite_SOURCES = AvlCalTestSuite.hpp AvlCalTestSuite.cpp
AvlCalTestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
AvlCalTestSuite_LDADD =
AvlCalTestSuite_LDFLAGS = \
		$(BOOST_LIBS) $(CPPUNIT_LIBS) $(EXTRACC_LIBS) $(STDAIR_LIBS) \
		$(top_builddir)/avlcal/core/libavlcal.la

