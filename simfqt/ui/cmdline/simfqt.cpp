/*!
 * \page ui_cmdline_simfqt_cpp Command-Line Utility to Demonstrate Typical Simfqt Usage
 * \code
 */
// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// Boost (Extended STL)
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/command/CmdBomManager.hpp>
// Stdair GNU Readline Wrapper
#include <stdair/ui/cmdline/SReadline.hpp>
// Simfqt
#include <simfqt/SIMFQT_Service.hpp>
#include <simfqt/config/simfqt-paths.hpp>


// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_SIMFQT_DEFAULT_LOG_FILENAME ("simfqt.log");

/** Default name and location for the (CSV) input file. */
const std::string K_SIMFQT_DEFAULT_FARE_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                        "/fare01.csv");

/**
 * Default for the input type. It can be either built-in or provided by an
 * input file. That latter must then be given with the -i option.
 */
const bool K_SIMFQT_DEFAULT_BUILT_IN_INPUT = false;

/**
 * Early return status (so that it can be differentiated from an error).
 */
const int K_SIMFQT_EARLY_RETURN_STATUS = 99;

/**
 * List of strings, representing the tokens as entered by the user on
 * a command-line.
 */
typedef std::vector<std::string> TokenList_T;

/**
 * Enumeration representing the command entered by the user on the command-line.
 */
struct Command_T {
  typedef enum {
    NOP = 0,
    QUIT,
    HELP,
    LIST,
    DISPLAY,
    PRICE,
    LAST_VALUE
  } Type_T;
};

// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], bool& ioIsBuiltin, 
                       stdair::Filename_T& ioFareInputFilename,
                       std::string& ioLogFilename) {

  // Default for the built-in input
  ioIsBuiltin = K_SIMFQT_DEFAULT_BUILT_IN_INPUT;

  // Declare a group of options that will be allowed only on command line
  boost::program_options::options_description generic ("Generic options");
  generic.add_options()
    ("prefix", "print installation prefix")
    ("version,v", "print version string")
    ("help,h", "produce help message");
    
  // Declare a group of options that will be allowed both on command
  // line and in config file
  boost::program_options::options_description config ("Configuration");
  config.add_options()
    ("builtin,b",
     "The sample BOM tree can be either built-in or parsed from an input file. That latter must then be given with the -f/--fare option")
    ("fare,f",
     boost::program_options::value< std::string >(&ioFareInputFilename)->default_value(K_SIMFQT_DEFAULT_FARE_INPUT_FILENAME),
     "(CSV) input file for the fare rules")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_SIMFQT_DEFAULT_LOG_FILENAME),
     "Filename for the logs")
    ;

  // Hidden options, will be allowed both on command line and
  // in config file, but will not be shown to the user.
  boost::program_options::options_description hidden ("Hidden options");
  hidden.add_options()
    ("copyright",
     boost::program_options::value< std::vector<std::string> >(),
     "Show the copyright (license)");
        
  boost::program_options::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  boost::program_options::options_description config_file_options;
  config_file_options.add(config).add(hidden);

  boost::program_options::options_description visible ("Allowed options");
  visible.add(generic).add(config);
        
  boost::program_options::positional_options_description p;
  p.add ("copyright", -1);
        
  boost::program_options::variables_map vm;
  boost::program_options::
    store (boost::program_options::command_line_parser (argc, argv).
           options (cmdline_options).positional(p).run(), vm);

  std::ifstream ifs ("fareQuote.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm); if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_SIMFQT_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_SIMFQT_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_SIMFQT_EARLY_RETURN_STATUS;
  }

  if (vm.count ("builtin")) {
    ioIsBuiltin = true;
  }
  const std::string isBuiltinStr = (ioIsBuiltin == true)?"yes":"no";
  std::cout << "The BOM should be built-in? " << isBuiltinStr << std::endl;

  if (ioIsBuiltin == false) {

    // The BOM tree should be built from parsing a fare (and O&D) file
    if (vm.count ("fare")) {
      ioFareInputFilename = vm["fare"].as< std::string >();
      std::cout << "Input fare filename is: " << ioFareInputFilename
                << std::endl;

    } else {
      // The built-in option is not selected. However, no fare file
      // is specified
      std::cerr << "Either one among the -b/--builtin and -f/--fare "
                << "options must be specified" << std::endl;
    }
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }
  
  return 0;
  
}

// //////////////////////////////////////////////////////////////////
void initReadline (swift::SReadline& ioInputReader) {

  // Prepare the list of my own completers
  std::vector<std::string> Completers;

  // The following is supported:
  // - "identifiers"
  // - special identifier %file - means to perform a file name completion
  Completers.push_back ("help");
  Completers.push_back ("list");
  Completers.push_back ("display %airport_code %airport_code %departure_date");
  Completers.push_back ("price");
  Completers.push_back ("quit");

  
  // Now register the completers.
  // Actually it is possible to re-register another set at any time
  ioInputReader.RegisterCompletions (Completers);
}

// //////////////////////////////////////////////////////////////////
Command_T::Type_T extractCommand (TokenList_T& ioTokenList) {
  Command_T::Type_T oCommandType = Command_T::LAST_VALUE;

  // Interpret the user input
  if (ioTokenList.empty() == false) {
    TokenList_T::iterator itTok = ioTokenList.begin();
    std::string& lCommand (*itTok);
    boost::algorithm::to_lower (lCommand);
    
    if (lCommand == "help") {
      oCommandType = Command_T::HELP;

    } else if (lCommand == "list") {
      oCommandType = Command_T::LIST;

    } else if (lCommand == "display") {
      oCommandType = Command_T::DISPLAY;

    } else if (lCommand == "price") {
      oCommandType = Command_T::PRICE;

    } else if (lCommand == "quit") {
      oCommandType = Command_T::QUIT;

    }

    // Remove the first token (the command), as the corresponding information
    // has been extracted in the form of the returned command type enumeration
    ioTokenList.erase (itTok);

  } else {
    oCommandType = Command_T::NOP;
  }

  return oCommandType;
}

// //////////////////////////////////////////////////////////////////
void parseFlightDateKey (const TokenList_T& iTokenList,
                         stdair::AirportCode_T& ioOrigin,
                         stdair::AirportCode_T& ioDestination,
                         stdair::Date_T& ioDepartureDate) {

  //
  const std::string kMonthStr[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  //
  unsigned short ioDepartureDateYear = ioDepartureDate.year();
  unsigned short ioDepartureDateMonth = ioDepartureDate.month();
  std::string ioDepartureDateMonthStr = kMonthStr[ioDepartureDateMonth-1];
  unsigned short ioDepartureDateDay = ioDepartureDate.day();
  
  // Interpret the user input
  if (iTokenList.empty() == false) {
    
    // Read the origin
    TokenList_T::const_iterator itTok = iTokenList.begin();
    if (itTok->empty() == false) {
      ioOrigin = *itTok;
      boost::algorithm::to_upper (ioOrigin);
    }

    // Read the destination
    ++itTok;
    if (itTok != iTokenList.end()) {
      
      if (itTok->empty() == false) {
      ioDestination  = *itTok;
      boost::algorithm::to_upper (ioDestination);
      }
      
    } else {
      return;
    }

    // Read the year for the departure date
    ++itTok;
    if (itTok != iTokenList.end()) {
      if (itTok->empty() == false) {
        try {

          ioDepartureDateYear = boost::lexical_cast<unsigned short> (*itTok);
          if (ioDepartureDateYear < 100) {
            ioDepartureDateYear += 2000;
          }

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The year of the flight departure date ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioDepartureDateYear << ") is kept. " << std::endl;
          return;
        }
      }

    } else {
      return;
    }

    // Read the month for the departure date
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {

          const boost::regex lMonthRegex ("^(\\d{1,2})$");
          const bool isMonthANumber = regex_match (*itTok, lMonthRegex);
        
          if (isMonthANumber == true) {
            const unsigned short lMonth =
              boost::lexical_cast<unsigned short> (*itTok);
            if (lMonth > 12) {
              throw boost::bad_lexical_cast();
            }
            ioDepartureDateMonthStr = kMonthStr[lMonth-1];

          } else {
            const std::string lMonthStr (*itTok);
            if (lMonthStr.size() < 3) {
              throw boost::bad_lexical_cast();
            }
            std::string lMonthStr1 (lMonthStr.substr (0, 1));
            boost::algorithm::to_upper (lMonthStr1);
            std::string lMonthStr23 (lMonthStr.substr (1, 2));
            boost::algorithm::to_lower (lMonthStr23);
            ioDepartureDateMonthStr = lMonthStr1 + lMonthStr23;
          }

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The month of the flight departure date ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioDepartureDateMonthStr << ") is kept. " << std::endl;
          return;
        }
      }

    } else {
      return;
    }

    // Read the day for the departure date
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {

          ioDepartureDateDay = boost::lexical_cast<unsigned short> (*itTok);

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The day of the flight departure date ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioDepartureDateDay << ") is kept. " << std::endl;
          return;
        }
      }

    } else {
      return;
    }

    // Re-compose the departure date
    std::ostringstream lDepartureDateStr;
    lDepartureDateStr << ioDepartureDateYear << "-" << ioDepartureDateMonthStr
                      << "-" << ioDepartureDateDay;

    try {

      ioDepartureDate =
        boost::gregorian::from_simple_string (lDepartureDateStr.str());

    } catch (boost::gregorian::bad_month& eCast) {
      std::cerr << "The flight departure date ('" << lDepartureDateStr.str()
                << "') cannot be understood. The default value ("
                << ioDepartureDate << ") is kept. " << std::endl;
      return;
    }

  }
}

// /////////////////////////////////////////////////////////
std::string toString (const TokenList_T& iTokenList) {
  std::ostringstream oStr;

  // Re-create the string with all the tokens, trimmed by read-line
  unsigned short idx = 0;
  for (TokenList_T::const_iterator itTok = iTokenList.begin();
       itTok != iTokenList.end(); ++itTok, ++idx) {
    if (idx != 0) {
      oStr << " ";
    }
    oStr << *itTok;
  }

  return oStr.str();
}

// /////////////////////////////////////////////////////////
TokenList_T extractTokenList (const TokenList_T& iTokenList,
                              const std::string& iRegularExpression) {
  TokenList_T oTokenList;

  // Re-create the string with all the tokens (which had been trimmed
  // by read-line)
  const std::string lFullLine = toString (iTokenList);

  // See the caller for the regular expression
  boost::regex expression (iRegularExpression);
  
  std::string::const_iterator start = lFullLine.begin();
  std::string::const_iterator end = lFullLine.end();

  boost::match_results<std::string::const_iterator> what;
  boost::match_flag_type flags = boost::match_default | boost::format_sed; 
  regex_search (start, end, what, expression, flags);
  
  // Put the matched strings in the list of tokens to be returned back
  // to the caller
  const unsigned short lMatchSetSize = what.size();
  for (unsigned short matchIdx = 1; matchIdx != lMatchSetSize; ++matchIdx) {
    const std::string lMatchedString (std::string (what[matchIdx].first,
                                                   what[matchIdx].second));
    //if (lMatchedString.empty() == false) {
    oTokenList.push_back (lMatchedString);
    //}
  }

  // DEBUG
  // std::cout << "After (token list): " << oTokenList << std::endl;

  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForOriDestDate (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:            airport_code airport_code year month day
   *   airport_code:    word (alpha{3})
   *   airport_code:    word (alpha{3})
   *   year:            number (digit{2,4})
   *   month:           (number (digit{1,2}) | word (alpha{3}))
   *   day:             number (digit{1,2})
   */
  const std::string lRegEx("^([[:alpha:]]{3})?"
                           "[[:space:]]*[-|/|,]?[[:space:]]*([[:alpha:]]{3})?"
                           "[[:space:]]*[-|/]*[[:space:]]*"
                           "([[:digit:]]{2,4})*[[:space:]]*[-|/][[:space:]]*"
                           "([[:alpha:]]{3}|[[:digit:]]{1,2})*[[:space:]]*[-|/][[:space:]]*"
                           "([[:digit:]]{1,2})*?$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // State whether the BOM tree should be built-in or parsed from an
  // input file
  bool isBuiltin;

  // Fare input file name
  stdair::Filename_T lFareInputFilename;

  // Readline history
  const unsigned int lHistorySize (100);
  const std::string lHistoryFilename ("fareDisplay.hist");
  const std::string lHistoryBackupFilename ("fareDisplay.hist.bak");

  // Default parameters for the interactive session
  stdair::AirportCode_T lInteractiveOrigin;
  stdair::AirportCode_T lInteractiveDestination;
  stdair::Date_T lInteractiveDepartureDate;

  // Output log File
  stdair::Filename_T lLogFilename;

  // Call the command-line option parser
  const int lOptionParserStatus =
    readConfiguration (argc, argv, isBuiltin, lFareInputFilename, lLogFilename);

  if (lOptionParserStatus == K_SIMFQT_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the fareQuote service
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile); 
  SIMFQT::SIMFQT_Service simfqtService (lLogParams);

  // DEBUG
  STDAIR_LOG_DEBUG ("Welcome to fareQuote display");

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {
    // Build the sample BOM tree (filled with fares) for Simfqt
    simfqtService.buildSampleBom();
  } else {
    // Build the BOM tree from parsing a fare file
    simfqtService.parseAndLoad (lFareInputFilename);
  }

  // DEBUG: Display the whole BOM tree
  const std::string& lCSVDump = simfqtService.csvDisplay();
  STDAIR_LOG_DEBUG (lCSVDump);

  // DEBUG
  STDAIR_LOG_DEBUG ("====================================================");
  STDAIR_LOG_DEBUG ("=       Beginning of the interactive session       =");
  STDAIR_LOG_DEBUG ("====================================================");

  // Initialise the GNU readline wrapper
  swift::SReadline lReader (lHistoryFilename, lHistorySize);
  initReadline (lReader);

  // Now we can ask user for a line
  std::string lUserInput;
  bool EndOfInput (false);
  Command_T::Type_T lCommandType (Command_T::NOP);
  
  while (lCommandType != Command_T::QUIT && EndOfInput == false) {

    stdair::TravelSolutionList_T lInteractiveTravelSolutionList;
    stdair::TravelSolutionStruct lInteractiveTravelSolution;

    // Update the default booking request
    // If there is an input file, we want CRS booking request (defined in stdair).
    // If not, we want the default booking request.
    const bool isCRSBookingRequest = !isBuiltin;
    const stdair::BookingRequestStruct& lInteractiveBookingRequest =
      simfqtService.buildBookingRequest (isCRSBookingRequest);

    // Update the default parameters for the following interactive session
    if (isBuiltin == true) {
      lInteractiveOrigin = "LHR";
      lInteractiveDestination = "SYD";
      lInteractiveDepartureDate = stdair::Date_T(2011,06,10);
      simfqtService.buildSampleTravelSolutions (lInteractiveTravelSolutionList);
    } else {
      lInteractiveOrigin = "SIN";
      lInteractiveDestination = "BKK";
      lInteractiveDepartureDate = stdair::Date_T(2010,01,30);
      //
      const std::string lBA9_SegmentDateKey ("SQ, 970, 2010-01-30, SIN, BKK, 07:10");

      // Add the segment date key to the travel solution
      lInteractiveTravelSolution.addSegment (lBA9_SegmentDateKey);

      // Add the travel solution to the list
      lInteractiveTravelSolutionList.push_back (lInteractiveTravelSolution);
    }
    
    // Prompt
    std::ostringstream oPromptStr;
    oPromptStr << "simfqt "
               << "> ";
    // The last parameter could be ommited
    TokenList_T lTokenListByReadline;
    lUserInput = lReader.GetLine (oPromptStr.str(), lTokenListByReadline,
                                  EndOfInput);

    // The history could be saved to an arbitrary file at any time
    lReader.SaveHistory (lHistoryBackupFilename);

    if (EndOfInput) {
      std::cout << std::endl;
      break;
    }

    // Interpret the user input
    lCommandType = extractCommand (lTokenListByReadline);
    
    switch (lCommandType) {

      // ////////////////////////////// Help ////////////////////////
    case Command_T::HELP: {
      std::cout << std::endl;
      std::cout << "Commands: " << std::endl;
      std::cout << " help" << "\t\t" << "Display this help" << std::endl;
      std::cout << " quit" << "\t\t" << "Quit the application" << std::endl;
      std::cout << " list" << "\t\t"
                << "List all the fare rule O&Ds and the corresponding date ranges" << std::endl;
      std::cout << " display" << "\t"
                << "Display all fare rules for an O&D and a departure date. \n" << "\t\t"
                << "If no O&D and no departure date specified, default values are used: \n"<< "\t\t"
                << "    'display " <<  lInteractiveOrigin << " " << lInteractiveDestination
                << " " << lInteractiveDepartureDate << "'"<< std::endl;
      std::cout << " price" << "\t\t"
                << "Price a travel solution" << std::endl;
      std::cout << std::endl;
      break;
    }
      
      // ////////////////////////////// Quit ////////////////////////
    case Command_T::QUIT: {
      break;
    }

      // ////////////////////////////// List /////////////////////////
    case Command_T::LIST: {
      
      //
      const std::string& lAirportPairDateListStr =
        simfqtService.list ();

      if (lAirportPairDateListStr.empty() == false) {
        std::cout << lAirportPairDateListStr << std::endl;
        STDAIR_LOG_DEBUG (lAirportPairDateListStr);

      } else {
        assert (isBuiltin == false);
        std::cerr << "There is no result for airport pairs and date ranges."
                  << "Make sure your input file is not empty."
                  << std::endl;
      }

      break;
    }

      // ////////////////////////////// Display /////////////////////////
    case Command_T::DISPLAY: {

      //
      TokenList_T lTokenList =
        extractTokenListForOriDestDate (lTokenListByReadline);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      parseFlightDateKey (lTokenList, lInteractiveOrigin,
                          lInteractiveDestination, lInteractiveDepartureDate);

      // Check whether the selected airportpair-date is valid
      const bool isAirportPairDateValid =
        simfqtService.check (lInteractiveOrigin, lInteractiveDestination,
                             lInteractiveDepartureDate);

      if (isAirportPairDateValid == false) {
        std::ostringstream oFDKStr;
        oFDKStr << "The airport pair/departure date: "
                << lInteractiveOrigin << ", " << lInteractiveDestination
                << " / " << lInteractiveDepartureDate
                << " does not correpond to any fare rule.\n"
                << "Make sure it exists with the 'list' command.";
        std::cout << oFDKStr.str() << std::endl;
        STDAIR_LOG_ERROR (oFDKStr.str());

        break;
      }
      
      //
      std::cout << "List of fare rules for "
                << lInteractiveOrigin << "-"
                << lInteractiveDestination << "/"
                << lInteractiveDepartureDate 
                << std::endl;

      const std::string& lFareRuleListStr =
        simfqtService.csvDisplay (lInteractiveOrigin,
                                  lInteractiveDestination,
                                  lInteractiveDepartureDate);

      if (lFareRuleListStr.empty() == false) {
        std::cout << lFareRuleListStr << std::endl;
        STDAIR_LOG_DEBUG (lFareRuleListStr);

      } else {
        std::cerr << "There is no result for "
                  << lInteractiveOrigin << "-"
                  << lInteractiveDestination << "/"
                  << lInteractiveDepartureDate
                  << ". Just type the list command without any parameter "
                  << "to see all the departure-dates for all the O&D"
                  << std::endl;
      }

      break;
    }
      
      // ////////////////////////////// Price ////////////////////////
    case Command_T::PRICE: {

      if (lInteractiveTravelSolutionList.size() < 1) {
        std::cerr << "There is no travel solution to fare quote."
                  << std::endl;
        break;
      }
      lInteractiveTravelSolution = lInteractiveTravelSolutionList.front();

      std::cout << "Booking request: << "
                << lInteractiveBookingRequest.display()  << " >>"
                << "\nTravel Solution: << "
                << lInteractiveTravelSolution.display() << " >>"
                << "\n********** \n"
                << "Fare quote"
                << "\n**********"
                << std::endl;

      // FareQuote the sample list of travel solutions
      simfqtService.quotePrices (lInteractiveBookingRequest,
                                 lInteractiveTravelSolutionList);

      lInteractiveTravelSolution = lInteractiveTravelSolutionList.front();

      std::cout << "Travel Solution: << "
                << lInteractiveTravelSolution.display() << " >>\n"
                << std::endl;

      break;
    }
      
      // /////////////////////////// Default / No value ///////////////////////
    case Command_T::NOP: {
      break;
    } 
    case Command_T::LAST_VALUE:
    default: {
      // DEBUG
      std::ostringstream oStr;
      oStr << "That command is not yet understood: '" << lUserInput << "'";

      STDAIR_LOG_DEBUG (oStr.str());
      std::cout << oStr.str() << std::endl;
    }
    }
  }

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the session. Exiting.");
  std::cout << "End of the session. Exiting." << std::endl;

  // Close the Log outputFile
  logOutputFile.close();

  /*
    Note: as that program is not intended to be run on a server in
    production, it is better not to catch the exceptions. When it
    happens (that an exception is throwned), that way we get the
    call stack.
  */

  return 0;	
}
