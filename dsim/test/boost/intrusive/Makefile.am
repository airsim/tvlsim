## test/boost/intrusive sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = intrusive bom
#TESTS = $(check_PROGRAMS)
TESTS =
XFAIL_TESTS = #IndexBuildingTestSuite

intrusive_SOURCES = intrusive.cpp
intrusive_CXXFLAGS = $(BOOST_CFLAGS)
intrusive_LDFLAGS = $(BOOST_LIBS)
intrusive_LDADD = 

bom_SOURCES = $(intru_bom_h_sources) $(intru_bom_cc_sources)
bom_CXXFLAGS = $(BOOST_CFLAGS)
bom_LDFLAGS = $(BOOST_LIBS)
bom_LDADD = 
