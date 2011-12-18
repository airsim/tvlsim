#
# DSim Tests
#

##
# Specify each test suite configuration

# * SimulationTestSuite Test Suite
module_test_add_suite (dsim SimulationTestSuite SimulationTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()

