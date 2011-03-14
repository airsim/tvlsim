## test/boost/date_time sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = date_time
#TESTS = $(check_PROGRAMS)
TESTS =
XFAIL_TESTS = #

date_time_SOURCES = date_time.cpp
date_time_CXXFLAGS= $(BOOST_CFLAGS)
date_time_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB)
date_time_LDADD = 

