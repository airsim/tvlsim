// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

/** Utility class for CPPUnit-based testing. */
class PartnerSimulationTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (PartnerSimulationTestSuite);
  CPPUNIT_TEST (partnerSimulation);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a partner simulation functionality. */
  void partnerSimulation();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  PartnerSimulationTestSuite ();
  
private:
  /** Test a partner simulation functionality. */
  void partnerSimulationHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (PartnerSimulationTestSuite);
