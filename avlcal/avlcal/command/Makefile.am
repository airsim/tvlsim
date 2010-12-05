## avlcal/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libavlcalcmd.la

libavlcalcmd_la_SOURCES = $(avlcal_cmd_h_sources) $(avlcal_cmd_cc_sources) 
libavlcalcmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libavlcalcmd_la_LIBADD =
libavlcalcmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(avlcal_cmd_h_sources)

