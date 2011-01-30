## test/trademgen sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
nofail_PRG = DemandGenerationTestSuite
fail_PRG =

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

DemandGenerationTestSuite_SOURCES = DemandGenerationTestSuite.cpp
DemandGenerationTestSuite_CXXFLAGS = $(BOOST_CFLAGS)  \
	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK $(STDAIR_CFLAGS)
DemandGenerationTestSuite_LDADD = 
DemandGenerationTestSuite_LDFLAGS = $(BOOST_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/trademgen/core/libtrademgen.la

