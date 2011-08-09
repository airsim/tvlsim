# airinv/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libairinv.la

libairinv_la_SOURCES = $(airinv_service_h_sources) $(airinv_service_cc_sources)
libairinv_la_LIBADD = \
	$(top_builddir)/airinv/basic/libairinvbas.la \
	$(top_builddir)/airinv/bom/libairinvbom.la \
	$(top_builddir)/airinv/factory/libairinvfac.la \
	$(top_builddir)/airinv/command/libairinvcmd.la \
	$(top_builddir)/airinv/service/libairinvsvc.la
libairinv_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/rmol/core/librmol.la \
	$(top_builddir)/airrac/core/libairrac.la \
	-version-info $(GENERIC_LIBRARY_VERSION)
