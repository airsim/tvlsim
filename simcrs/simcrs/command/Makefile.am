## simcrs/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libsimcrscmd.la

libsimcrscmd_la_SOURCES = $(simcrs_cmd_h_sources) $(simcrs_cmd_cc_sources) 
libsimcrscmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libsimcrscmd_la_LIBADD =
libsimcrscmd_la_LDFLAGS = $(BOOST_LIBS) \
		$(top_builddir)/airinv/core/libairinv.la

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(simcrs_cmd_h_sources)

