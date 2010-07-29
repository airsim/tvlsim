## test/boost/asio sub-directory
include $(top_srcdir)/Makefile.common

##

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = client synServer asynServer

client_SOURCES = client.cpp networkFunctions.cpp
client_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
client_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
client_LDADD =

synServer_SOURCES = synServer.cpp
synServer_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
synServer_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
synServer_LDADD =

asynServer_SOURCES = asynServer.cpp
asynServer_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
asynServer_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) $(BOOST_ASIO_LIB)
asynServer_LDADD =