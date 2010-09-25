## test/boost/random sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST =

#
check_PROGRAMS = random_demo random_order_stats

random_demo_SOURCES = random_demo.cpp
random_demo_CXXFLAGS = $(BOOST_CFLAGS)
random_demo_LDADD = $(BOOST_LIBS)

random_order_stats_SOURCES = random_order_stats.cpp
random_order_stats_CXXFLAGS = $(BOOST_CFLAGS)
random_order_stats_LDADD = $(BOOST_LIBS)
