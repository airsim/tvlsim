#
# DSim Tests
#
module_set_name (dsim)


##
# Specify each test suite configuration

# * SimulationTestSuite Test Suite
module_test_add_suite (SimulationTestSuite SimulationTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()
