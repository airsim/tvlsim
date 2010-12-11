# travelccm/batches sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in


# Binaries (batches)
bin_PROGRAMS = travelccm

travelccm_SOURCES = $(travelccm_batches_h_sources) \
		$(travelccm_batches_cc_sources)
travelccm_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
travelccm_LDADD = 
travelccm_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/travelccm/core/libtravelccm.la
