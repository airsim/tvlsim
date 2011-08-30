# simcrs/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libsimcrs.la

libsimcrs_la_SOURCES = $(simcrs_service_h_sources) $(simcrs_service_cc_sources)
libsimcrs_la_LIBADD = \
	$(top_builddir)/simcrs/basic/libsimcrsbas.la \
	$(top_builddir)/simcrs/bom/libsimcrsbom.la \
	$(top_builddir)/simcrs/factory/libsimcrsfac.la \
	$(top_builddir)/simcrs/command/libsimcrscmd.la \
	$(top_builddir)/simcrs/service/libsimcrssvc.la
libsimcrs_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_FILESYSTEM_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airsched/core/libairsched.la \
	$(top_builddir)/airinv/core/libairinv.la \
	$(top_builddir)/simfqt/core/libsimfqt.la \
	-version-info $(GENERIC_LIBRARY_VERSION)

