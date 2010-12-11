# simfqt/batches
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

# Binaries (batches)
bin_PROGRAMS = simfqt

simfqt_SOURCES = $(simfqt_batches_h_sources) $(simfqt_batches_cc_sources)
simfqt_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
simfqt_LDADD =
simfqt_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la
