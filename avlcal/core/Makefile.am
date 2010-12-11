# avlcal/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libavlcal.la

libavlcal_la_SOURCES = $(avlcal_service_h_sources) $(avlcal_service_cc_sources)
libavlcal_la_LIBADD = \
	$(top_builddir)/avlcal/basic/libavlcalbas.la \
	$(top_builddir)/avlcal/bom/libavlcalbom.la \
	$(top_builddir)/avlcal/factory/libavlcalfac.la \
	$(top_builddir)/avlcal/command/libavlcalcmd.la \
	$(top_builddir)/avlcal/service/libavlcalsvc.la
libavlcal_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)

