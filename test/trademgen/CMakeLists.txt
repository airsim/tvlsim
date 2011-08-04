#
# TraDemGen Tests
#
module_set_name (trademgen)


##
# Specify each test suite configuration

# * TraDemGen Test Suite
module_test_add_suite (TrademgenTest DemandGenerationTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()
