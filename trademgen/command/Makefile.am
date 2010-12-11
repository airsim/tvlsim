## trademgen/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtrademgencmd.la

libtrademgencmd_la_SOURCES = $(cmd_h_sources) $(cmd_cc_sources) 
libtrademgencmd_la_CXXFLAGS = $(SOCI_CFLAGS) $(STDAIR_CFLAGS)
libtrademgencmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(cmd_h_sources)

