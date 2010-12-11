# simcrs/batches
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

# Binaries (batches)
bin_PROGRAMS = simcrs

simcrs_SOURCES = $(simcrs_batches_h_sources) $(simcrs_batches_cc_sources)
simcrs_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
simcrs_LDADD =
simcrs_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la \
	$(top_builddir)/airsched/core/libairsched.la \
	$(top_builddir)/simfqt/core/libsimfqt.la \
	$(top_builddir)/simcrs/core/libsimcrs.la

