# airsched/batches
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

# Library (for the batches)
lib_LTLIBRARIES = libairschedbatch.la

libairschedbatch_la_SOURCES = $(airsched_batch_lib_h_sources) \
	$(airsched_batch_lib_cc_sources)
libairschedbatch_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libairschedbatch_la_LIBADD =
libairschedbatch_la_LDFLAGS =

# Binaries (batches)
bin_PROGRAMS = airsched

#
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${bin_PROGRAMS:%=%.log}

#
airsched_SOURCES = $(airsched_batches_h_sources) $(airsched_batches_cc_sources)
airsched_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airsched_LDADD = 
airsched_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airsched/batches/libairschedbatch.la \
	$(top_builddir)/airsched/core/libairsched.la

