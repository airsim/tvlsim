# airsched/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libairsched.la

libairsched_la_SOURCES = \
	$(airsched_service_h_sources) $(airsched_service_cc_sources)
libairsched_la_LIBADD = \
	$(top_builddir)/airsched/basic/libairschedbas.la \
	$(top_builddir)/airsched/bom/libairschedbom.la \
	$(top_builddir)/airsched/factory/libairschedfac.la \
	$(top_builddir)/airsched/command/libairschedcmd.la \
	$(top_builddir)/airsched/service/libairschedsvc.la
libairsched_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_FILESYSTEM_LIB) $(STDAIR_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)

