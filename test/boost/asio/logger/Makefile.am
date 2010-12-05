## test/asio/logger sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = daytime_client

daytime_client_SOURCES = stream_socket_service.hpp \
	logger_service.hpp logger_service.cpp basic_logger.hpp logger.hpp \
	daytime_client.cpp
daytime_client_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
daytime_client_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
daytime_client_LDADD =

EXTRA_DIST =
