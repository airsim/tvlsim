## travelccm/factory sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtravelccmfac.la

libtravelccmfac_la_SOURCES = $(travelccm_fac_h_sources) \
		$(travelccm_fac_cc_sources) 
libtravelccmfac_la_CXXFLAGS = $(STDAIR_CFLAGS)
libtravelccmfac_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/factory
#pkginclude_HEADERS = $(travelccm_fac_h_sources)
