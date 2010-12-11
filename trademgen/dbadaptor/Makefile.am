## trademgen/dbadaptor sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtrademgendba.la

libtrademgendba_la_SOURCES = $(dba_h_sources) $(dba_cc_sources) 
libtrademgendba_la_CXXFLAGS = $(SOCI_CFLAGS) $(STDAIR_LIBS)
libtrademgendba_la_LIBADD =
libtrademgendba_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/dbadaptor
#pkginclude_HEADERS = $(dba_h_sources)

