// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

/** Utility class for CPPUnit-based testing. */
class AvlCalTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (AvlCalTestSuite);
  CPPUNIT_TEST (simpleAvlCal);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a simple availability calculation functionality. */
  void simpleAvlCal();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  AvlCalTestSuite ();
  
private:
  /** Test a simple availability calculation functionality. */
  void simpleAvlCalHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (AvlCalTestSuite);
