## dsim/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libdsimsvc.la

libdsimsvc_la_SOURCES = $(dsim_svc_h_sources) $(dsim_svc_cc_sources) 
libdsimsvc_la_CXXFLAGS = $(BOOST_CFLAGS)


#pkgincludedir = $(includedir)/@PACKAGE@/service
#pkginclude_HEADERS = $(svc_h_sources)
