// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

/** Utility class for CPPUnit-based testing. */
class InventoryTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (InventoryTestSuite);
  CPPUNIT_TEST (simpleInventory);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a simple inventory functionality. */
  void simpleInventory();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  InventoryTestSuite ();
  
private:
  /** Test a simple inventory functionality. */
  void simpleInventoryHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (InventoryTestSuite);
