## test/mpi sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = mpi

#mpi_c_SOURCES = mpi_c.cpp
#mpi_c_CXXFLAGS = $(MPICH2_CFLAGS)
#mpi_c_LDFLAGS = $(MPICH2_LIB)
#mpi_c_LDADD =

mpi_SOURCES = mpi.cpp
mpi_CXXFLAGS = $(BOOST_CFLAGS) $(MPICH2_CFLAGS)
mpi_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
mpi_LDADD =

EXTRA_DIST =
