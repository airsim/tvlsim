## airsched/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libairschedcmd.la

libairschedcmd_la_SOURCES = $(airsched_cmd_h_sources) $(airsched_cmd_cc_sources) 
libairschedcmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libairschedcmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(airsched_cmd_h_sources)

