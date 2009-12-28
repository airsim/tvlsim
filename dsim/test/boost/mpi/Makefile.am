## test/mpi sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = mpi_c mpi mpi_p2p

# mpi_c must be compiled with mpic++
# Note: mpic++/mpiCC is a wrapper around the MPI C++ compiler, and knows
#       where MPI is installed. So, there is no need to specify any other 
#       CFLAGS or LDFLAGS at that stage.
mpi_c: mpi_c.cpp
	mpic++ -o $@ $<

mpi_SOURCES = mpi.cpp
mpi_CXXFLAGS = $(BOOST_CFLAGS) $(MPICH2_CFLAGS)
mpi_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
mpi_LDADD =

mpi_p2p_SOURCES = mpi_p2p.cpp
mpi_p2p_CXXFLAGS = $(BOOST_CFLAGS) $(MPICH2_CFLAGS)
mpi_p2p_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
mpi_p2p_LDADD =

EXTRA_DIST =
