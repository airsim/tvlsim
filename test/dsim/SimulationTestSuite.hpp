// STL
#include <iosfwd>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

/** Utility class for CPPUnit-based testing. */
class SimulationTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (SimulationTestSuite);
  CPPUNIT_TEST (simpleSimulation);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test a simple simulation functionality. */
  void simpleSimulation();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  SimulationTestSuite ();
  
private:
  /** Test a simple simulation functionality. */
  void simpleSimulationHelper();

protected:
  std::stringstream _describeKey;
};

CPPUNIT_TEST_SUITE_REGISTRATION (SimulationTestSuite);
