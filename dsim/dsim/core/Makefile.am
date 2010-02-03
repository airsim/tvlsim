# core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libdsim.la

libdsim_la_SOURCES = $(service_h_sources) $(service_cc_sources)
libdsim_la_LIBADD = \
	$(top_builddir)/dsim/basic/libbas.la \
	$(top_builddir)/dsim/bom/libbom.la \
	$(top_builddir)/dsim/factory/libfac.la \
	$(top_builddir)/dsim/command/libcmd.la \
	$(top_builddir)/dsim/service/libsvc.la
libdsim_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_FILESYSTEM_LIB) \
	$(top_builddir)/stdair/core/libstdair.la \
	$(top_builddir)/airsched/core/libairsched.la \
	$(top_builddir)/simcrs/core/libsimcrs.la \
	$(top_builddir)/trademgen/core/libtrademgen.la \
	-version-info $(GENERIC_LIBRARY_VERSION)
