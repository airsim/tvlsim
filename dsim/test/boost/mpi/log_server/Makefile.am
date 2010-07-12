## test/boost/mpi/log_server sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

#
SUBDIRS = 

EXTRA_DIST =

#
check_PROGRAMS = log_server client


# mpi_c must be compiled with mpic++
# Note: mpic++/mpiCC is a wrapper around the MPI C++ compiler, and knows
#       where MPI is installed. So, there is no need to specify any other 
#       CFLAGS or LDFLAGS at that stage.
mpi_c_SOURCES = mpi_c.cpp
mpi_c: $(mpi_c_SOURCES)
	mpic++ -o $@ $<

#
log_server_SOURCES = log_server.cpp
log_server_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
log_server_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
log_server_LDADD =

#
client_SOURCES = client.cpp
client_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
client_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
client_LDADD =

