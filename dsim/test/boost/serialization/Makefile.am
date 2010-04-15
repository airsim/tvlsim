## test/boost/serialization sub-directory
include $(top_srcdir)/Makefile.common

##
SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST =

#
check_PROGRAMS = serial1 serial_simple

serial1_SOURCES = serial1.cpp
serial1_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
serial1_LDFLAGS = $(BOOST_LIBS) $(BOOST_SERIALIZATION_LIB)
serial1_LDADD =

serial_simple_SOURCES = serial_simple.cpp
serial_simple_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
serial_simple_LDFLAGS = $(BOOST_LIBS) $(BOOST_SERIALIZATION_LIB)
serial_simple_LDADD =

