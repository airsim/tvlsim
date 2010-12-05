// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
//#include <stdair/bom/Inventory.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <simfqt/command/FareParserHelper.hpp>
#include <simfqt/command/FareParser.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////
  void FareParser::fareRuleGeneration (const stdair::Filename_T& iFilename,
                                       stdair::BomRoot& ioBomRoot) {

    // Initialise the Fare file parser.
    FareRuleFileParser lFareRuleFileParser (ioBomRoot, iFilename);

    // Parse the CSV-formatted fare input file generate the
    // corresponding fare rules and fulfill the WorldSchedule.
    lFareRuleFileParser.generateFareRules ();

  }

}
