## test sub-directory of the DSim project
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
# As every component can already be tested independently, for instance
# by a Continuous Integration (CI) framework such as Hudson/Jenkins,
# it is useless to retest them here. Uncomment the following line only
# when you have cannot test those components by another way.
#SUBDIRS = boost trademgen simcrs travelccm dsim
SUBDIRS = boost dsim

EXTRA_DIST =

##
check_PROGRAMS =
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #
