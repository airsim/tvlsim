## airsched/basic sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libairschedbas.la

libairschedbas_la_SOURCES = $(airsched_bas_h_sources) $(airsched_bas_cc_sources) 
libairschedbas_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libairschedbas_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/basic
#pkginclude_HEADERS = $(airsched_bas_h_sources)

