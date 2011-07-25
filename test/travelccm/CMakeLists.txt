#
# TravelCCM Tests
#
module_set_name (travelccm)


##
# Specify each test suite configuration

# * TravelChoiceTestSuite Test Suite
module_test_add_suite (TravelChoiceTestSuite TravelChoiceTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()
