## test/airinv sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = InventoryTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

InventoryTestSuite_SOURCES = InventoryTestSuite.hpp InventoryTestSuite.cpp
InventoryTestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS) \
	$(EXTRACC_CFLAGS) $(STDAIR_CFLAGS)
InventoryTestSuite_LDADD =
InventoryTestSuite_LDFLAGS = \
	$(BOOST_LIBS) $(CPPUNIT_LIBS) $(EXTRACC_LIBS) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la

