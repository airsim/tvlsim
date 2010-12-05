# simfqt/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libsimfqt.la

libsimfqt_la_SOURCES = $(simfqt_service_h_sources) $(simfqt_service_cc_sources)
libsimfqt_la_LIBADD = \
	$(top_builddir)/simfqt/basic/libsimfqtbas.la \
	$(top_builddir)/simfqt/bom/libsimfqtbom.la \
	$(top_builddir)/simfqt/factory/libsimfqtfac.la \
	$(top_builddir)/simfqt/command/libsimfqtcmd.la \
	$(top_builddir)/simfqt/service/libsimfqtsvc.la
libsimfqt_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)

# Header files
#pkginclude_HEADERS = $(simfqt_service_h_sources)
#nobase_nodist_pkginclude_HEADERS = $(top_builddir)/simfqt/config.h
