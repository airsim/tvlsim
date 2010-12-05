// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

/** Utility class for CPPUnit-based testing. */
class CRSTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (CRSTestSuite);
  CPPUNIT_TEST (simpleCRS);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a simple crs functionality. */
  void simpleCRS();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  CRSTestSuite ();
  
private:
  /** Test a simple crs functionality. */
  void simpleCRSHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (CRSTestSuite);
