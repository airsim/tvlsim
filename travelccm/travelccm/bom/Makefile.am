## travelccm/bom sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libtravelccmbom.la

libtravelccmbom_la_SOURCES = $(travelccm_bom_h_sources) \
		$(travelccm_bom_cc_sources) 
libtravelccmbom_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libtravelccmbom_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/bom
#pkginclude_HEADERS = $(travelccm_bom_h_sources)
