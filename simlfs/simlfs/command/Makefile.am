## simlfs/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libsimlfscmd.la

libsimlfscmd_la_SOURCES = $(simlfs_cmd_h_sources) $(simlfs_cmd_cc_sources) 
libsimlfscmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libsimlfscmd_la_LIBADD =
libsimlfscmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(simlfs_cmd_h_sources)
