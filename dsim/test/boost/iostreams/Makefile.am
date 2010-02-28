## test/boost/iostreams sub-directory
include $(top_srcdir)/Makefile.common

##
SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST =

#
check_PROGRAMS = filereader

filereader_SOURCES = filereader.cpp
filereader_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
filereader_LDFLAGS = $(BOOST_LIBS) $(BOOST_ASIO_LIB) $(BOOST_IOSTREAMS_LIB)
filereader_LDADD =
