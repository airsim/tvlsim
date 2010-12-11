// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

/** Utility class for CPPUnit-based testing. */
class LFSTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (LFSTestSuite);
  CPPUNIT_TEST (simpleLFS);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a simple fare quote functionality. */
  void simpleLFS();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  LFSTestSuite ();
  
private:
  /** Test a simple fare quote functionality. */
  void simpleLFSHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (LFSTestSuite);
