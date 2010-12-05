## airinv/service sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libairinvsvc.la

libairinvsvc_la_SOURCES = $(airinv_svc_h_sources) $(airinv_svc_cc_sources) 
libairinvsvc_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libairinvsvc_la_LIBADD =
libairinvsvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(airinv_svc_h_sources)

