# airinv/batches
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
# Binaries (batches)
bin_PROGRAMS = airinv_parseInventory

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${bin_PROGRAMS:%=%.log}

##
#
airinv_parseInventory_SOURCES = $(airinv_batches_h_sources) \
	$(airinv_batches_cc_sources)
airinv_parseInventory_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airinv_parseInventory_LDADD =
airinv_parseInventory_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la
