## travelccm/basic sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtravelccmbas.la

libtravelccmbas_la_SOURCES = $(travelccm_bas_h_sources) \
		$(travelccm_bas_cc_sources) 
libtravelccmbas_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libtravelccmbas_la_LDFLAGS =

# Headers to be exported/installed
#pkgincludedir = $(includedir)/travelccm/basic
#pkginclude_HEADERS = $(travelccm_bas_h_sources)
