# airrac/batches
include $(top_srcdir)/Makefile.common

## Source directory

MAINTAINERCLEANFILES = Makefile.in


# Binaries (batches)
bin_PROGRAMS = airrac

#
airrac_SOURCES = airrac.cpp
airrac_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airrac_LDADD =
airrac_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airrac/core/libairrac.la
