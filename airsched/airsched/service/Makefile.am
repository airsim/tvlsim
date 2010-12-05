## airsched/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libairschedsvc.la

libairschedsvc_la_SOURCES = $(airsched_svc_h_sources) $(airsched_svc_cc_sources) 
libairschedsvc_la_CXXFLAGS = $(STDAIR_CFLAGS)
libairschedsvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(airsched_svc_h_sources)

