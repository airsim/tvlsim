## travelccm/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtravelccmcmd.la

libtravelccmcmd_la_SOURCES = $(travelccm_cmd_h_sources) \
		$(travelccm_cmd_cc_sources) 
libtravelccmcmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libtravelccmcmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(travelccm_cmd_h_sources)
