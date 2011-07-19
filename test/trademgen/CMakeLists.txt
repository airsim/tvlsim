#
# TraDemGen Tests
#
if (Boost_FOUND)

  # CMake macro (defined in config/add_test_suite.cmake)
  include (add_test_suite)

  enable_testing() 

  # * TraDemGen Test Suite
  #file (GLOB trademgentest_SOURCES *.cpp)
  set (TEST_TARGET TrademgenTest)
  set (trademgentest_SOURCES DemandGenerationTestSuite.cpp)
  add_test_suite (${TEST_TARGET} "${trademgentest_SOURCES}"
    ${TRADEMGEN_LIB_TARGET} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  # * Next test suite
  # ...

  # Don't forget to add the test suite to the DEPENDS argument list of
  # the 'check' target within the test/CMakeLists.txt file
  add_custom_target (check${TEST_TARGET} COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS ${TEST_TARGET})

endif()
