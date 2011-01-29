// Boost Unit Test Framework (UTF)
#include <boost/test/unit_test.hpp>

namespace boost_utf = boost::unit_test;

// Simple test case 1
void test_case1() {
  BOOST_CHECK (1 == 1);
}
// Simple test case 2
void test_case2() {
  if (1 != 2) {
    BOOST_ERROR ("Test case 2 failing...");
  }
}
// Simple test case 3
void test_case3() {
  BOOST_CHECK_MESSAGE (1 == 1, "Test case 3");
}
// Simple test case 4
void test_case4() {
  BOOST_CHECK_EQUAL (1, 2);
}


// /////////////// Main: Unit Test Suite //////////////
boost_utf::test_suite* init_unit_test_suite (int argc, char* argv[]) {
 boost_utf::test_suite* lTest1_ptr = BOOST_TEST_SUITE ("test_suite1");
  assert (lTest1_ptr != NULL);
  lTest1_ptr->add (BOOST_TEST_CASE (&test_case1));
  lTest1_ptr->add (BOOST_TEST_CASE (&test_case2));
  
  boost_utf::test_suite* lTest2_ptr = BOOST_TEST_SUITE ("test_suite2");
  assert (lTest2_ptr != NULL);
  lTest2_ptr->add (BOOST_TEST_CASE (&test_case3));
  lTest2_ptr->add (BOOST_TEST_CASE (&test_case4));
  
  boost_utf::framework::master_test_suite().add (lTest1_ptr);
  boost_utf::framework::master_test_suite().add (lTest2_ptr);
  boost_utf::framework::master_test_suite().add (BOOST_TEST_CASE (&test_case1));
  
  return NULL;
}
