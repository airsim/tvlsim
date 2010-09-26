## test/boost/random sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST =

##
# Test library
noinst_LTLIBRARIES = librndmgr.la

librndmgr_la_SOURCES = $(librndmgr_h_sources) $(librndmgr_cc_sources)
librndmgr_la_CXXFLAGS = $(BOOST_CFLAGS)
librndmgr_la_LIBADD = 
librndmgr_la_LDFLAGS =

#
check_PROGRAMS = random_demo random_order_stats

random_demo_SOURCES = random_demo.cpp
random_demo_CXXFLAGS = $(BOOST_CFLAGS)
random_demo_LDFLAGS = $(BOOST_LIBS)

random_order_stats_SOURCES = random_order_stats.cpp
random_order_stats_CXXFLAGS = $(BOOST_CFLAGS)
random_order_stats_LDFLAGS = $(BOOST_LIBS) $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(builddir)/librndmgr.la
