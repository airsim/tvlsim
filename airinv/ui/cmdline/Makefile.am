# airinv/ui/cmdline
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
# Binaries (command-line interactive programs)
bin_PROGRAMS = airinv

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${bin_PROGRAMS:%=%.log} \
	${bin_PROGRAMS:%=%.hist} ${bin_PROGRAMS:%=%.hist.bak}

##
#
airinv_SOURCES = $(airinv_cmdline_h_sources) $(airinv_cmdline_cc_sources)
airinv_CXXFLAGS = $(READLINE_CFLAGS) $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airinv_LDADD =
airinv_LDFLAGS = $(READLINE_LIBS) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la
