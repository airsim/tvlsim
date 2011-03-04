# dsim/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libdsim.la

libdsim_la_SOURCES = $(dsim_service_h_sources) $(dsim_service_cc_sources)
libdsim_la_LIBADD = \
	$(top_builddir)/dsim/basic/libdsimbas.la \
	$(top_builddir)/dsim/bom/libdsimbom.la \
	$(top_builddir)/dsim/factory/libdsimfac.la \
	$(top_builddir)/dsim/command/libdsimcmd.la \
	$(top_builddir)/dsim/service/libdsimsvc.la
libdsim_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_FILESYSTEM_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/simcrs/core/libsimcrs.la \
	$(top_builddir)/trademgen/core/libtrademgen.la \
	$(top_builddir)/travelccm/core/libtravelccm.la \
	-version-info $(GENERIC_LIBRARY_VERSION)

