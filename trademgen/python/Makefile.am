## trademgen/python sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in
DISTCLEANFILES = pytrademgen.py

EXTRA_DIST = pytrademgen.py

# Library
lib_LTLIBRARIES = libpytrademgen.la

libpytrademgen_la_SOURCES = pytrademgen.cpp
libpytrademgen_la_CXXFLAGS = $(PYTHON_CFLAGS) $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libpytrademgen_la_LIBADD =
libpytrademgen_la_LDFLAGS = $(PYTHON_LIBS) $(PYTHON_ADD_LIBS) \
	$(BOOST_PYTHON_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/trademgen/core/libtrademgen.la \
	-version-info $(GENERIC_LIBRARY_VERSION)

