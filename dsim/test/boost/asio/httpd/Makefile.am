## test/asio/httpd sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = httpd_server

httpd_server_SOURCES = \
	header.hpp connection.hpp mime_types.hpp reply.hpp request.hpp \
	request_handler.hpp request_parser.hpp server.hpp \
	mime_types.cpp connection.cpp posix_main.cpp reply.cpp request_handler.cpp \
	request_parser.cpp server.cpp win_main.cpp
httpd_server_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
httpd_server_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
httpd_server_LDADD =

EXTRA_DIST =
