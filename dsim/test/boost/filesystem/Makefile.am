## test/boost/filesystem sub-directory
include $(top_srcdir)/Makefile.common

#
MAINTAINERCLEANFILES = Makefile.in

#
SUBDIRS =

EXTRA_DIST =

#
check_PROGRAMS = simple_ls file_size process_jam_log \
				path_test wide_test operations_test fstream_test

#
simple_ls_SOURCES = simple_ls.cpp
simple_ls_CXXFLAGS = $(BOOST_CFLAGS)
simple_ls_LDADD =
simple_ls_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM_LIB)

#
file_size_SOURCES = file_size.cpp
file_size_CXXFLAGS = $(BOOST_CFLAGS)
file_size_LDADD =
file_size_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM_LIB)

#
process_jam_log_SOURCES = tiny_xml.hpp tiny_xml.cpp process_jam_log.cpp
process_jam_log_CXXFLAGS = $(BOOST_CFLAGS)
process_jam_log_LDADD =
process_jam_log_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM_LIB)

#
path_test_SOURCES = path_test.cpp
path_test_CXXFLAGS = $(BOOST_CFLAGS)
path_test_LDADD =
path_test_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM_LIB)

#
wide_test_SOURCES = wide_test.cpp
wide_test_CXXFLAGS = $(BOOST_CFLAGS)
wide_test_LDADD =
wide_test_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM_LIB)

#
operations_test_SOURCES = operations_test.cpp
operations_test_CXXFLAGS = $(BOOST_CFLAGS)
operations_test_LDADD =
operations_test_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM_LIB)

#
fstream_test_SOURCES = fstream_test.cpp
fstream_test_CXXFLAGS = $(BOOST_CFLAGS)
fstream_test_LDADD =
fstream_test_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM_LIB)

