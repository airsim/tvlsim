# simfqt/ui/cmdline
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
# Binaries (command-line interactive programs)
bin_PROGRAMS = simfqt

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${bin_PROGRAMS:%=%.log} \
	${bin_PROGRAMS:%=%.hist} ${bin_PROGRAMS:%=%.hist.bak}

##
#
simfqt_SOURCES = $(simfqt_cmdline_h_sources) $(simfqt_cmdline_cc_sources)
simfqt_CXXFLAGS = $(READLINE_CFLAGS) $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
simfqt_LDADD =
simfqt_LDFLAGS = $(READLINE_LIBS) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la
