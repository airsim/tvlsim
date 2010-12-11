## simfqt/service sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libsimfqtsvc.la

libsimfqtsvc_la_SOURCES= $(simfqt_svc_h_sources) $(simfqt_svc_cc_sources) 
libsimfqtsvc_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libsimfqtsvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(simfqt_svc_h_sources)
