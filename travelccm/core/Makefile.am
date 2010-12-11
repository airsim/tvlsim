# travelccm/core sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libtravelccm.la

libtravelccm_la_SOURCES = $(travelccm_service_h_sources) \
		$(travelccm_service_cc_sources)
libtravelccm_la_LIBADD = \
	$(top_builddir)/travelccm/basic/libtravelccmbas.la \
	$(top_builddir)/travelccm/bom/libtravelccmbom.la \
	$(top_builddir)/travelccm/factory/libtravelccmfac.la \
	$(top_builddir)/travelccm/command/libtravelccmcmd.la \
	$(top_builddir)/travelccm/service/libtravelccmsvc.la
libtravelccm_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)
