##travelccm/ command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtravelccmsvc.la

libtravelccmsvc_la_SOURCES = $(travelccm_svc_h_sources) \
		$(travelccm_svc_cc_sources) 
libtravelccmsvc_la_CXXFLAGS = $(STDAIR_CFLAGS)
libtravelccmsvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(travelccm_svc_h_sources)
