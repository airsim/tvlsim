// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

class DemandGenerationTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (DemandGenerationTestSuite);
  CPPUNIT_TEST (simpleEventGeneration);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a simple event generation functionality. */
  void simpleEventGeneration();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  DemandGenerationTestSuite ();
  
private:
  /** Test a simple event generation functionality. */
  void simpleEventGenerationHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (DemandGenerationTestSuite);
