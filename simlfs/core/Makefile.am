# simlfs/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libsimlfs.la

libsimlfs_la_SOURCES = $(simlfs_service_h_sources) $(simlfs_service_cc_sources)
libsimlfs_la_LIBADD = \
	$(top_builddir)/simlfs/basic/libsimlfsbas.la \
	$(top_builddir)/simlfs/bom/libsimlfsbom.la \
	$(top_builddir)/simlfs/factory/libsimlfsfac.la \
	$(top_builddir)/simlfs/command/libsimlfscmd.la \
	$(top_builddir)/simlfs/service/libsimlfssvc.la
libsimlfs_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)

# Header files
#pkginclude_HEADERS = $(simlfs_service_h_sources)
#nobase_nodist_pkginclude_HEADERS = $(top_builddir)/simlfs/config.h
