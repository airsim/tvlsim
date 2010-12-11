## simcrs/service sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libsimcrssvc.la

libsimcrssvc_la_SOURCES = $(simcrs_svc_h_sources) $(simcrs_svc_cc_sources) 
libsimcrssvc_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libsimcrssvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(simcrs_svc_h_sources)

