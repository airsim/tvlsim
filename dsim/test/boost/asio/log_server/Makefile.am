## test/boost/asio sub-directory
include $(top_srcdir)/Makefile.common

##

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = client server

client_SOURCES = client.cpp
client_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
client_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
client_LDADD =

server_SOURCES = server.cpp
server_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
server_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
server_LDADD =

