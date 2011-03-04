// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/service/Logger.hpp>
// AirSched
#include <airinv/command/DCPParserHelper.hpp>
#include <airinv/command/DCPParser.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  void DCPParser::DCPRuleGeneration (const stdair::Filename_T& iFilename,
                                     stdair::BomRoot& ioBomRoot) {

    // Initialise the DCP file parser
    DCPRuleFileParser lDCPRuleFileParser (ioBomRoot, iFilename);

    // Parse the CSV-formatted DCP input file and generate the
    // corresponding DCP events
    lDCPRuleFileParser.generateDCPRules();
  }

}
