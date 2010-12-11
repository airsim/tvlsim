## trademgen/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtrademgensvc.la

libtrademgensvc_la_SOURCES = $(svc_h_sources) $(svc_cc_sources) 
libtrademgensvc_la_CXXFLAGS = $(SOCI_CFLAGS) $(STDAIR_CFLAGS)
libtrademgensvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(svc_h_sources)

