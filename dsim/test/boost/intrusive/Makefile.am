## test/boost/intrusive sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = intrusive
#TESTS = $(check_PROGRAMS)
TESTS =
XFAIL_TESTS = #IndexBuildingTestSuite

intrusive_SOURCES = intrusive.cpp
intrusive_CXXFLAGS= $(BOOST_CFLAGS)
intrusive_LDFLAGS = $(BOOST_LIBS)
intrusive_LDADD = 
