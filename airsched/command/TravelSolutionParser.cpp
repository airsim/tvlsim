// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <cassert>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasConst_TravelSolution.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <airsched/command/TravelSolutionParser.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  bool TravelSolutionParser::
  parseInputFileAndBuildBom (const std::string& iInputFileName) {
    bool hasReadBeenSuccessful = false;

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iInputFileName);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The input file, '" << iInputFileName
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Open the input file
    std::ifstream inputFile (iInputFileName.c_str());
    if (! inputFile) {
      STDAIR_LOG_ERROR ("Can not open input file '" << iInputFileName << "'");
      throw new stdair::FileNotFoundException ("Can not open input file '"
                                               + iInputFileName + "'");
    }
    
    char buffer[80];
    double dval = 0.0;
    std::string dvalStr;
    short i = 1;
    bool hasAllPArams = true;

    stdair::AirportCode_T dAirport;
    stdair::AirportCode_T aAirport;
    stdair::Date_T depDate;
    stdair::Duration_T depTime;
    stdair::Duration_T arTime;
    stdair::Duration_T dur;
    //bool Ref;
    stdair::AirlineCode_T airline;
    stdair::CabinCode_T cabin;
    //stdair::FlightNumber_T flightNum;
    //stdair::Fare_T fare;
    //int lagsNum;
    //bool SNS;
    //bool change;
    
    while (inputFile.getline (buffer, sizeof (buffer), ';')) {
      std::istringstream iStringStr (buffer);

      bool hasRead = false;

      if (i == 1) {
        hasAllPArams = true;
      }

      if (i>=1 && i<=14) {
        hasRead = (iStringStr >> dvalStr);
      }

      if (i == 15) {
        hasRead = (iStringStr >> dval);
      }

      if (hasRead) {
        if (i == 1) {
          dAirport = dvalStr;

        } else if (i == 2) {
          aAirport = dvalStr;
          // std::cout << "City Pair = '" << dAiport
          // << "-" << aAirport << "'" << std::endl;

        } else if (i == 3) {
          depDate = boost::gregorian::from_simple_string (dvalStr);
          // std::cout << "Date = '" << depDate << "'" << std::endl;

        } else if (i == 4) {
          depTime = boost::posix_time::duration_from_string (dvalStr);

        } else if (i == 5) {
          arTime = boost::posix_time::duration_from_string (dvalStr);

        } else if (i == 6) {
          dur = boost::posix_time::duration_from_string (dvalStr);

        } else if (i == 7) {
          //if (dvalStr == "refundable fare")
          //  Ref = true;
          //else Ref  = false;

        } else if (i == 8) {
          airline = dvalStr;

        } else if (i == 9) {
          cabin = dvalStr;

        } else if (i == 10) {
          //flightNum = dval;

        } else if (i == 11) {
          //fare = dval;

        } else if (i == 12) {
          //lagsNum = dval;

        } else if (i == 13) {
          //if (dvalStr == "Saturday Nigth Stay mandatory")
          //  SNS = true;
          //else SNS = false;

        } else if (i == 14) {
          //if (dvalStr == "changeable fare")
          //  change = true;
          //else change = false;
          i = 0;
        }

        //
        ++i;

      } else {
        hasAllPArams = false;
      }
    }

    if (hasAllPArams && i == 1) {
      STDAIR_LOG_DEBUG ("Successfully read");
    }
    
    //
    if (!inputFile.eof()) {
      STDAIR_LOG_ERROR ("Problem when reading input file '" << iInputFileName
                        << "'");
      return hasReadBeenSuccessful;
    }

    //
    hasReadBeenSuccessful = true;
    return hasReadBeenSuccessful;
  }
  
}
