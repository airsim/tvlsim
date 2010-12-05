## test/boost/mpi/master_slave sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

#
SUBDIRS = 

EXTRA_DIST =

#
check_PROGRAMS = master slave


#
master_SOURCES = master.cpp
master_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
master_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
master_LDADD =

#
slave_SOURCES = slave.cpp
slave_CXXFLAGS = $(BOOST_CFLAGS) $(MPIGEN_CFLAGS)
slave_LDFLAGS = $(BOOST_LIBS) $(BOOST_MPI_LIB)
slave_LDADD =
