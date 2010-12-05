## airinv/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libairinvcmd.la

libairinvcmd_la_SOURCES = $(airinv_cmd_h_sources) $(airinv_cmd_cc_sources) 
libairinvcmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libairinvcmd_la_LIBADD =
libairinvcmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(airinv_cmd_h_sources)
