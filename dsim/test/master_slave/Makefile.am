# batches
include $(top_srcdir)/Makefile.common

## Source directory

MAINTAINERCLEANFILES = Makefile.in


# Binaries (batches)
bin_PROGRAMS = 	master slave log_server

master_SOURCES = master.cpp
master_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
master_LDADD =
master_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_LIBS) $(BOOST_MPI_LIB) \
	$(top_builddir)/stdair/core/libstdair.la \
	$(top_builddir)/airsched/core/libairsched.la


slave_SOURCES = slave.cpp
slave_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
slave_LDADD =
slave_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_LIBS) $(BOOST_MPI_LIB) \
	$(top_builddir)/stdair/core/libstdair.la \
	$(top_builddir)/airsched/core/libairsched.la


log_server_SOURCES = log_server.cpp
log_server_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
log_server_LDADD =
log_server_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_LIBS) $(BOOST_MPI_LIB) \
	$(top_builddir)/stdair/core/libstdair.la \
	$(top_builddir)/airsched/core/libairsched.la
