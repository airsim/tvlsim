## test/asio/logger sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = logd

logd_SOURCES = logd.cpp
logd_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_CFLAGS)
logd_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) \
	$(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_ASIO_LIB)
logd_LDADD =

EXTRA_DIST =
