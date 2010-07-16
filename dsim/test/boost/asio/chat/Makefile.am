## test/boost/asio sub-directory
include $(top_srcdir)/Makefile.common

##

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = chat_client chat_server

chat_client_SOURCES = chat_client.cpp
chat_client_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
chat_client_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
chat_client_LDADD =

chat_server_SOURCES = chat_server.cpp
chat_server_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
chat_server_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
chat_server_LDADD =

