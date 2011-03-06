# trademgen/batches
include $(top_srcdir)/Makefile.common

##
# Binaries (batches)
bin_PROGRAMS = trademgen trademgen_with_db
generated_FILES = request.csv

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = $(generated_FILES) ${bin_PROGRAMS:%=%.log}

##
#
trademgen_SOURCES = trademgen.cpp
trademgen_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
trademgen_LDADD =
trademgen_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/trademgen/core/libtrademgen.la

#
trademgen_with_db_SOURCES = trademgen_with_db.cpp
trademgen_with_db_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
trademgen_with_db_LDADD =
trademgen_with_db_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(SOCI_LIBS) \
	$(STDAIR_LIBS) \
	$(top_builddir)/trademgen/core/libtrademgen.la

