## avlcal/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libavlcalsvc.la

libavlcalsvc_la_SOURCES = $(avlcal_svc_h_sources) $(avlcal_svc_cc_sources) 
libavlcalsvc_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libavlcalsvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(avlcal_svc_h_sources)

