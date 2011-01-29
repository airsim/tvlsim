// Boost Unit Test Framework (UTF)
#define BOOST_TEST_MODULE ProgressTest
#include <boost/test/unit_test.hpp>

namespace boost_utf = boost::unit_test;

// Simple test case
void free_test_function() {
  BOOST_CHECK (true /* test assertion */);
}

// /////////////// Main: Unit Test Suite //////////////
BOOST_AUTO_TEST_SUITE( test_suite1 )

BOOST_AUTO_TEST_CASE (test_case1) {
  for (int i=0; i < 10000; i++ ) {
    boost_utf::framework::master_test_suite().
      add (BOOST_TEST_CASE (&free_test_function));
  }
}

BOOST_AUTO_TEST_SUITE_END()
