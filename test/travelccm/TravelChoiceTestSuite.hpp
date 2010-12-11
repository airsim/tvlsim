// STL
#include <sstream>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

class TravelChoiceTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (TravelChoiceTestSuite);
  CPPUNIT_TEST (testTravelChoice);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test the Optimisation functionality. */
  void testTravelChoice ();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  TravelChoiceTestSuite ();
  
protected:
  std::stringstream _describeKey;
};
CPPUNIT_TEST_SUITE_REGISTRATION (TravelChoiceTestSuite);
