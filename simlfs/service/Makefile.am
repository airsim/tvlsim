## simlfs/service sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libsimlfssvc.la

libsimlfssvc_la_SOURCES = $(simlfs_svc_h_sources) $(simlfs_svc_cc_sources) 
libsimlfssvc_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libsimlfssvc_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(simlfs_svc_h_sources)
