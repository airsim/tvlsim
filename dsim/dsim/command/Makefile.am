## dsim/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libdsimcmd.la

libdsimcmd_la_SOURCES = $(dsim_cmd_h_sources) $(dsim_cmd_cc_sources) 
libdsimcmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(SOCI_CFLAGS) $(STDAIR_CFLAGS)
libdsimcmd_la_LIBADD =
libdsimcmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(dsim_cmd_h_sources)
