## test/trademgen sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = DemandGenerationTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

DemandGenerationTestSuite_SOURCES = \
	DemandGenerationTestSuite.hpp DemandGenerationTestSuite.cpp
# test.cpp 
DemandGenerationTestSuite_CXXFLAGS = $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
DemandGenerationTestSuite_LDADD = 
DemandGenerationTestSuite_LDFLAGS = $(BOOST_LIBS) $(CPPUNIT_LIBS) \
	$(EXTRACC_LIBS) $(STDAIR_LIBS) \
	$(top_builddir)/trademgen/core/libtrademgen.la

