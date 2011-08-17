// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
//#include <stdair/bom/Inventory.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <simfqt/command/FareParserHelper.hpp>
#include <simfqt/command/FareParser.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////
  void FareParser::fareRuleGeneration (const stdair::Filename_T& iFilename,
                                       stdair::BomRoot& ioBomRoot) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The fare input file, '" << iFilename
                        << "', can not be retrieved on the file-system");
      throw FareInputFileNotFoundException ("The fare input file '" + iFilename
                                            + "' does not exist or can not "
                                            "be read");
    }

    // Initialise the fare file parser.
    FareRuleFileParser lFareRuleFileParser (ioBomRoot, iFilename);

    // Parse the CSV-formatted fare input file and generate the
    // corresponding fare rules.
    lFareRuleFileParser.generateFareRules ();

  }

}
