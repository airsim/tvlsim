# simfqt/ui/cmdline
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
# Binaries (command-line interactive programs)
bin_PROGRAMS = fareQuote

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${bin_PROGRAMS:%=%.log} \
	${bin_PROGRAMS:%=%.hist} ${bin_PROGRAMS:%=%.hist.bak}

##
#
fareQuote_SOURCES = $(simfqt_cmdline_h_sources) $(simfqt_cmdline_cc_sources)
fareQuote_CXXFLAGS = $(READLINE_CFLAGS) $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
fareQuote_LDADD =
fareQuote_LDFLAGS = $(READLINE_LIBS) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la
