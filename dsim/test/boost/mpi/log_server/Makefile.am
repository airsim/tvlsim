## test/boost/mpi/log_server sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

#
SUBDIRS = 

EXTRA_DIST =

#
check_PROGRAMS = log_server client

#
log_server_SOURCES = log_server.cpp
log_server_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
log_server_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB) \
			$(top_builddir)/stdair/core/libstdair.la \
			$(top_builddir)/airsched/core/libairsched.la

#
client_SOURCES = client.cpp
client_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
client_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
client_LDADD =

