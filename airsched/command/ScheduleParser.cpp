// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomRoot.hpp>
// AirSched
#include <airsched/command/SegmentPathGenerator.hpp>
#include <airsched/command/ScheduleParserHelper.hpp>
#include <airsched/command/ScheduleParser.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  void ScheduleParser::generateInventories (const stdair::Filename_T& iFilename,
                                            stdair::BomRoot& ioBomRoot) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iFilename);

    if (doesExistAndIsReadable == false) {
      throw ScheduleInputFileNotFoundException ("The schedule file " + iFilename
                                                + " does not exist or can not "
                                                "be read");
    }

    // Initialise the Flight-Period file parser.
    FlightPeriodFileParser lFlightPeriodParser (ioBomRoot, iFilename);

    // Parse the CSV-formatted schedule input file, and generate the
    // corresponding Inventories for the airlines.
    lFlightPeriodParser.generateInventories();
    
    // Build the network from the schedule.
    SegmentPathGenerator::createSegmentPathNetwork (ioBomRoot);
  }

}
