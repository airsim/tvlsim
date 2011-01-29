## test/boost/unit_test sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =
##

#
nofail_PRG = test_progress # test_manual_registration
fail_PRG = test_automatic_registration

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

#
#test_manual_registration_SOURCES = test_manual_registration.cpp
#test_manual_registration_CXXFLAGS= $(BOOST_CFLAGS) \
#	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK
#test_manual_registration_LDFLAGS = $(BOOST_LIBS) \
#	$(BOOST_UNIT_TEST_FRAMEWORK_LIB)
#test_manual_registration_LDADD = 

test_automatic_registration_SOURCES = test_automatic_registration.cpp
test_automatic_registration_CXXFLAGS= $(BOOST_CFLAGS) \
	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK
test_automatic_registration_LDFLAGS = $(BOOST_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIB)
test_automatic_registration_LDADD = 

test_progress_SOURCES = test_progress.cpp
test_progress_CXXFLAGS= $(BOOST_CFLAGS) \
	-DBOOST_TEST_MAIN -DBOOST_TEST_DYN_LINK
test_progress_LDFLAGS = $(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)
test_progress_LDADD = 

