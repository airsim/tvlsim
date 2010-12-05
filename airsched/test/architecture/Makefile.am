## test/architecture sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

check_PROGRAMS = archi pointers accu
# crossinc
#TESTS = $(check_PROGRAMS)
TESTS =
XFAIL_TESTS = #IndexBuildingTestSuite

archi_SOURCES = archi.cpp
archi_CXXFLAGS= $(BOOST_CFLAGS)
archi_LDFLAGS = $(BOOST_LIBS)
archi_LDADD = 

pointers_SOURCES = pointers.cpp
pointers_CXXFLAGS= $(BOOST_CFLAGS)
pointers_LDFLAGS = $(BOOST_LIBS)
pointers_LDADD = 

accu_SOURCES = accu.cpp
accu_CXXFLAGS= $(BOOST_CFLAGS)
accu_LDFLAGS = $(BOOST_LIBS)
accu_LDADD = 

#crossinc_SOURCES = A.hpp B.hpp AB.cpp
#crossinc_CXXFLAGS=
#crossinc_LDFLAGS =
#crossinc_LDADD = 

