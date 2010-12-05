## test/simfqt sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = FQTTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

FQTTestSuite_SOURCES = FQTTestSuite.hpp FQTTestSuite.cpp
FQTTestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
FQTTestSuite_LDADD =
FQTTestSuite_LDFLAGS = $(CPPUNIT_LIBS) $(EXTRACC_LIBS) $(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la
