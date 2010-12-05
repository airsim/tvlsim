# trademgen/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libtrademgen.la

libtrademgen_la_SOURCES = $(service_h_sources) $(service_cc_sources)
libtrademgen_la_LIBADD = \
	$(top_builddir)/trademgen/basic/libtrademgenbas.la \
	$(top_builddir)/trademgen/bom/libtrademgenbom.la \
	$(top_builddir)/trademgen/factory/libtrademgenfac.la \
	$(top_builddir)/trademgen/dbadaptor/libtrademgendba.la \
	$(top_builddir)/trademgen/command/libtrademgencmd.la \
	$(top_builddir)/trademgen/service/libtrademgensvc.la
libtrademgen_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)

