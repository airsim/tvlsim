# airinv/batches
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
# Binaries (batches)
bin_PROGRAMS = parseInventory

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${bin_PROGRAMS:%=%.log}

##
#
parseInventory_SOURCES = $(airinv_batches_h_sources) $(airinv_batches_cc_sources)
parseInventory_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
parseInventory_LDADD =
parseInventory_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la
