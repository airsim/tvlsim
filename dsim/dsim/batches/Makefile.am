# batches
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in


# Binaries (batches)
bin_PROGRAMS = simulate

simulate_SOURCES = $(batches_h_sources) $(batches_cc_sources)
simulate_CXXFLAGS = $(BOOST_CFLAGS)
simulate_LDADD =
simulate_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(SOCI_LIBS) \
	$(top_builddir)/dsim/core/libdsim.la
