// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomRoot.hpp>
// AirSched
#include <airsched/command/OnDParserHelper.hpp>
#include <airsched/command/OnDParser.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  void OnDParser::generateOnDPeriods (const stdair::Filename_T& iFilename,
                                      stdair::BomRoot& ioBomRoot) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iFilename);

    if (doesExistAndIsReadable == false) {
      throw OnDInputFileNotFoundException ("The O&D file " + iFilename
                                           + " does not exist or can not be "
                                           "read");
    }

    // Initialise the O&D-Period file parser.
    OnDPeriodFileParser lOnDPeriodParser (iFilename, ioBomRoot);

    // Parse the CSV-formatted O&D input file, and generate the
    // corresponding O&D-Period for the airlines.
    lOnDPeriodParser.generateOnDPeriods();
  }

}
