## command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

noinst_LTLIBRARIES= libcmd.la

libcmd_la_SOURCES= $(cmd_h_sources) $(cmd_cc_sources) 
libcmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(SOCI_CFLAGS)
libcmd_la_LIBADD =
libcmd_la_LDFLAGS = $(BOOST_LIBS) $(SOCI_LIBS) \
		$(top_builddir)/simcrs/core/libsimcrs.la \
		$(top_builddir)/trademgen/core/libtrademgen.la


#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(cmd_h_sources)
