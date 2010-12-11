// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

/** Utility class for CPPUnit-based testing. */
class FQTTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (FQTTestSuite);
  CPPUNIT_TEST (simpleFQT);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a simple fare quote functionality. */
  void simpleFQT();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  FQTTestSuite ();
  
private:
  /** Test a simple fare quote functionality. */
  void simpleFQTHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (FQTTestSuite);
