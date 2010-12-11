## airsched/bom sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libairschedbom.la

libairschedbom_la_SOURCES = $(airsched_bom_h_sources) $(airsched_bom_cc_sources) 
libairschedbom_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libairschedbom_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/bom
#pkginclude_HEADERS = $(airsched_bom_h_sources)

