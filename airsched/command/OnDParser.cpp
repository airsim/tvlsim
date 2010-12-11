// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/bom/Inventory.hpp>
// AirSched
#include <airsched/command/OnDParserHelper.hpp>
#include <airsched/command/OnDParser.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  void OnDParser::
  generateOnDPeriods (const stdair::Filename_T& iFilename,
                      stdair::BomRoot& ioBomRoot) {

    // Initialise the O&D-Period file parser.
    OnDPeriodFileParser lOnDPeriodParser (iFilename, ioBomRoot);

    // Parse the CSV-formatted schedule input file, and generate the
    // corresponding O&D-Period for the airlines.
    lOnDPeriodParser.generateOnDPeriods ();
  }

}
