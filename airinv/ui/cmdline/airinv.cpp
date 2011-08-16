/*!
 * \page batch_airinv_cpp Command-Line Utility to Demonstrate Typical AirInv Usage
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
#include <boost/swap.hpp>
#include <boost/algorithm/string/case_conv.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/AIRINV_Master_Service.hpp>
#include <airinv/config/airinv-paths.hpp>
// GNU Readline Wrapper
#include <airinv/ui/cmdline/SReadline.hpp>

// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_AIRINV_DEFAULT_LOG_FILENAME ("airinv.log");

/**
 * Default name and location for the (CSV) input files.
 */
const std::string K_AIRINV_DEFAULT_INVENTORY_FILENAME (STDAIR_SAMPLE_DIR
                                                       "/invdump01.csv");
/**
 * Default name and location for the (CSV) input files.
 */
const std::string K_AIRINV_DEFAULT_SCHEDULE_FILENAME (STDAIR_SAMPLE_DIR
                                                      "/schedule01.csv");
/**
 * Default name and location for the (CSV) input files.
 */
const std::string K_AIRINV_DEFAULT_OND_FILENAME (STDAIR_SAMPLE_DIR
                                                 "/ond01.csv");

/**
 * Default name and location for the (CSV) input files.
 */
const std::string K_AIRINV_DEFAULT_YIELD_FILENAME (STDAIR_SAMPLE_DIR
                                                   "/yieldstore01.csv");

/**
 * Default for the input type. It can be either built-in or provided by an
 * input file. That latter must then be given with the -i option.
 */
const bool K_AIRINV_DEFAULT_BUILT_IN_INPUT = false;

/**
 * Default for the input type. The BOM tree can be built from either an
 * inventory dump or from a schedule file (and, potentially, an O&D list).
 */
const bool K_AIRINV_DEFAULT_FOR_SCHEDULE = false;

/**
 * Early return status (so that it can be differentiated from an error).
 */
const int K_AIRINV_EARLY_RETURN_STATUS = 99;

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
    SELECT,
    SELL,
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

/**
 * Read and parse the command line options.
 */
int readConfiguration (int argc, char* argv[],
                       bool& ioIsBuiltin, bool& ioIsForSchedule,
                       stdair::Filename_T& ioInventoryFilename,
                       stdair::Filename_T& ioScheduleInputFilename,
                       stdair::Filename_T& ioODInputFilename,
                       stdair::Filename_T& ioYieldInputFilename,
                       std::string& ioLogFilename) {
  // Default for the built-in input
  ioIsBuiltin = K_AIRINV_DEFAULT_BUILT_IN_INPUT;

  // Default for the inventory or schedule option
  ioIsForSchedule = K_AIRINV_DEFAULT_FOR_SCHEDULE;

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
     "The sample BOM tree can be either built-in or parsed from an input file. That latter must then be given with the -i/--inventory or -s/--schedule option")
    ("for_schedule,f",
     "The BOM tree should be built from a schedule file (instead of from an inventory dump)")
    ("inventory,i",
     boost::program_options::value< std::string >(&ioInventoryFilename)->default_value(K_AIRINV_DEFAULT_INVENTORY_FILENAME),
     "(CSV) input file for the inventory")
    ("schedule,s",
     boost::program_options::value< std::string >(&ioScheduleInputFilename)->default_value(K_AIRINV_DEFAULT_SCHEDULE_FILENAME),
     "(CSV) input file for the schedule")
    ("ond,o",
     boost::program_options::value< std::string >(&ioODInputFilename)->default_value(K_AIRINV_DEFAULT_OND_FILENAME),
     "(CSV) input file for the O&D")
    ("yield,y",
     boost::program_options::value< std::string >(&ioYieldInputFilename)->default_value(K_AIRINV_DEFAULT_YIELD_FILENAME),
     "(CSV) input file for the yield")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_AIRINV_DEFAULT_LOG_FILENAME),
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

  std::ifstream ifs ("airinv.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_AIRINV_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_AIRINV_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_AIRINV_EARLY_RETURN_STATUS;
  }

  if (vm.count ("builtin")) {
    ioIsBuiltin = true;
  }
  const std::string isBuiltinStr = (ioIsBuiltin == true)?"yes":"no";
  std::cout << "The BOM should be built-in? " << isBuiltinStr << std::endl;

  if (vm.count ("for_schedule")) {
    ioIsForSchedule = true;
  }
  const std::string isForScheduleStr = (ioIsForSchedule == true)?"yes":"no";
  std::cout << "The BOM should be built from schedule? " << isForScheduleStr
            << std::endl;

  if (ioIsBuiltin == false) {

    if (ioIsForSchedule == false) {
      // The BOM tree should be built from parsing an inventory dump
      if (vm.count ("inventory")) {
        ioInventoryFilename = vm["inventory"].as< std::string >();
        std::cout << "Input inventory filename is: " << ioInventoryFilename
                  << std::endl;

      } else {
        // The built-in option is not selected. However, no inventory dump
        // file is specified
        std::cerr << "Either one among the -b/--builtin, -i/--inventory or "
                  << " -f/--for_schedule and -s/--schedule options "
                  << "must be specified" << std::endl;
      }

    } else {
      // The BOM tree should be built from parsing a schedule (and O&D) file
      if (vm.count ("schedule")) {
        ioScheduleInputFilename = vm["schedule"].as< std::string >();
        std::cout << "Input schedule filename is: " << ioScheduleInputFilename
                  << std::endl;

      } else {
        // The built-in option is not selected. However, no schedule file
        // is specified
        std::cerr << "Either one among the -b/--builtin, -i/--inventory or "
                  << " -f/--for_schedule and -s/--schedule options "
                  << "must be specified" << std::endl;
      }

      if (vm.count ("ond")) {
        ioODInputFilename = vm["ond"].as< std::string >();
        std::cout << "Input O&D filename is: " << ioODInputFilename << std::endl;
      }

      if (vm.count ("yield")) {
        ioYieldInputFilename = vm["yield"].as< std::string >();
        std::cout << "Input yield filename is: " << ioYieldInputFilename << std::endl;
      }
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
  Completers.push_back ("list %airline_code %flight_number");
  Completers.push_back ("select %airline_code %flight_number %flight_date");
  Completers.push_back ("display");
  Completers.push_back ("sell %booking_class %party_size %origin %destination");
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
    std::string lCommand (*itTok);
    boost::algorithm::to_lower (lCommand);
    
    if (lCommand == "help") {
      oCommandType = Command_T::HELP;

    } else if (lCommand == "list") {
      oCommandType = Command_T::LIST;

    } else if (lCommand == "display") {
      oCommandType = Command_T::DISPLAY;

    } else if (lCommand == "select") {
      oCommandType = Command_T::SELECT;

    } else if (lCommand == "sell") {
      oCommandType = Command_T::SELL;

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
void parseFlightKey (const TokenList_T& iTokenList,
                     stdair::AirlineCode_T& ioAirlineCode,
                     stdair::FlightNumber_T& ioFlightNumber) {
  // Interpret the user input
  if (iTokenList.empty() == false) {

    // Read the airline code
    TokenList_T::const_iterator itTok = iTokenList.begin();
    if (itTok->empty() == false) {
      ioAirlineCode = *itTok;
      boost::algorithm::to_upper (ioAirlineCode);
    }

    // Read the flight-number
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {

          ioFlightNumber = boost::lexical_cast<stdair::FlightNumber_T> (*itTok);

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The flight number ('" << *itTok
                    << "') cannot be understood. "
                    << "The default value (all) is kept."
                    << std::endl;
          return;
        }
      }

    } else {
      return;
    }
  }
}

// //////////////////////////////////////////////////////////////////
void parseFlightDateKey (const TokenList_T& iTokenList,
                         stdair::AirlineCode_T& ioAirlineCode,
                         stdair::FlightNumber_T& ioFlightNumber,
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

    // Read the airline code
    TokenList_T::const_iterator itTok = iTokenList.begin();
    if (itTok->empty() == false) {
      ioAirlineCode = *itTok;
      boost::algorithm::to_upper (ioAirlineCode);
    }

    // Read the flight-number
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {

          ioFlightNumber = boost::lexical_cast<stdair::FlightNumber_T> (*itTok);

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The flight number ('" << *itTok
                    << "') cannot be understood. "
                    << "The default value (all) is kept."
                    << std::endl;
          return;
        }
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

// //////////////////////////////////////////////////////////////////
void parseBookingClassKey (const TokenList_T& iTokenList,
                           stdair::ClassCode_T& ioBookingClass,
                           stdair::PartySize_T& ioPartySize,
                           stdair::AirportCode_T& ioOrigin,
                           stdair::AirportCode_T& ioDestination) {
  // Interpret the user input
  if (iTokenList.empty() == false) {

    // Read the booking class
    TokenList_T::const_iterator itTok = iTokenList.begin();
    if (itTok->empty() == false) {
      ioBookingClass = *itTok;
      boost::algorithm::to_upper (ioBookingClass);
    }
      
    // Read the party size
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {

          ioPartySize = boost::lexical_cast<stdair::PartySize_T> (*itTok);

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The party size ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioPartySize << ") is kept." << std::endl;
          return;
        }
      }

    } else {
      return;
    }

    // Read the origin
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        ioOrigin = *itTok;
        boost::algorithm::to_upper (ioOrigin);
      }

    } else {
      return;
    }

    // Read the destination
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        ioDestination = *itTok;
        boost::algorithm::to_upper (ioDestination);
      }

    } else {
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
TokenList_T extractTokenListForFlight (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:            airline_code[ ]?flight_number departure_date
   *   airline_code:    word (alpha{2,3})
   *   flight_number:   number (digit{1,4})
   */
  const std::string lRegEx ("^([[:alpha:]]{2,3})?"
                            "[[:space:]]*([[:digit:]]{1,4})?$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForFlightDate (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:            airline_code[ ]?flight_number departure_date
   *   airline_code:    word (alpha{2,3})
   *   flight_number:   number (digit{1,4})
   *   departure_date:  year[/- ]?month[/- ]?day
   *   year:            number (digit{2,4})
   *   month:           (number (digit{1,2}) | word (alpha{3}))
   *   day:             number (digit{1,2})
   */
  const std::string lRegEx("^([[:alpha:]]{2,3})?"
                           "[[:space:]]*([[:digit:]]{1,4})?"
                           "[/ ]*"
                           "([[:digit:]]{2,4})?[/-]?[[:space:]]*"
                           "([[:alpha:]]{3}|[[:digit:]]{1,2})?[/-]?[[:space:]]*"
                           "([[:digit:]]{1,2})?$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForClass (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:            class_code party_size origin destination
   *   class_code:      word (alpha)
   *   party_size:      number (digit{1,3})
   *   origin:          word (alpha{3})
   *   destination:     word (alpha{3})
   */
  const std::string lRegEx ("^([[:alpha:]])?"
                            "[[:space:]]*([[:digit:]]{1,3})?"
                            "[[:space:]]*([[:alpha:]]{3})?"
                            "[[:space:]]*([[:alpha:]]{3})?$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    


// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // State whether the BOM tree should be built-in or parsed from an
  // input file
  bool isBuiltin;
  bool isForSchedule;

  // Input file names
  stdair::Filename_T lInventoryFilename;
  stdair::Filename_T lScheduleInputFilename;
  stdair::Filename_T lODInputFilename;
  stdair::Filename_T lYieldInputFilename;

  // Readline history
  const unsigned int lHistorySize (100);
  const std::string lHistoryFilename ("airinv.hist");
  const std::string lHistoryBackupFilename ("airinv.hist.bak");

  // Default parameters for the interactive session
  stdair::AirlineCode_T lLastInteractiveAirlineCode;
  stdair::FlightNumber_T lLastInteractiveFlightNumber;
  stdair::Date_T lLastInteractiveDate;
  stdair::AirlineCode_T lInteractiveAirlineCode;
  stdair::FlightNumber_T lInteractiveFlightNumber;
  stdair::Date_T lInteractiveDate;
  stdair::AirportCode_T lInteractiveOrigin;
  stdair::AirportCode_T lInteractiveDestination;
  stdair::ClassCode_T lInteractiveBookingClass;
  stdair::PartySize_T lInteractivePartySize;
  
  // Parameters for the sale
  std::string lSegmentDateKey;

  // Output log File
  stdair::Filename_T lLogFilename;

  // Call the command-line option parser
  const int lOptionParserStatus =
    readConfiguration (argc, argv, isBuiltin, isForSchedule, lInventoryFilename,
                       lScheduleInputFilename, lODInputFilename,
                       lYieldInputFilename, lLogFilename);

  if (lOptionParserStatus == K_AIRINV_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the inventory service
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  AIRINV::AIRINV_Master_Service airinvService (lLogParams);

  // DEBUG
  STDAIR_LOG_DEBUG ("Welcome to AirInv");

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {

    // Build the sample BOM tree for RMOL
    airinvService.buildSampleBom();

    // Update the default parameters for the following interactive session
    lInteractiveAirlineCode = "BA";
    lInteractiveFlightNumber = 9;
    lInteractiveDate = stdair::Date_T (2011, 06, 10);
    lInteractiveBookingClass = "Q";
    lInteractivePartySize = 2;
    lInteractiveOrigin = "LHR";
    lInteractiveDestination = "SYD";

  } else {
    if (isForSchedule == true) {
      // Build the BOM tree from parsing a schedule file (and O&D list)
      airinvService.parseAndLoad (lScheduleInputFilename, lODInputFilename,
                                  lYieldInputFilename);

      // Update the default parameters for the following interactive session
      lInteractiveAirlineCode = "SQ";
      lInteractiveFlightNumber = 11;
      lInteractiveDate = stdair::Date_T (2010, 01, 15);
      lInteractiveBookingClass = "Y";
      lInteractivePartySize = 2;
      lInteractiveOrigin = "SIN";
      lInteractiveDestination = "BKK";

    } else {
      // Build the BOM tree from parsing an inventory dump file
      airinvService.parseAndLoad (lInventoryFilename);

      // Update the default parameters for the following interactive session
      lInteractiveAirlineCode = "SV";
      lInteractiveFlightNumber = 5;
      lInteractiveDate = stdair::Date_T (2010, 03, 11);
      lInteractiveBookingClass = "Y";
      lInteractivePartySize = 2;
      lInteractiveOrigin = "KBP";
      lInteractiveDestination = "JFK";
    }
  }

  // Save the last state
  lLastInteractiveAirlineCode = lInteractiveAirlineCode;
  lLastInteractiveFlightNumber = lInteractiveFlightNumber;
  lLastInteractiveDate = lInteractiveDate;

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
    // Prompt
    std::ostringstream oPromptStr;
    oPromptStr << "airinv "
               << lInteractiveAirlineCode << lInteractiveFlightNumber
               << " / " << lInteractiveDate
               << "> ";
    // Call read-line, which will fill the list of tokens
    TokenList_T lTokenListByReadline;
    lUserInput = lReader.GetLine (oPromptStr.str(), lTokenListByReadline,
                                  EndOfInput);

    // The history can be saved to an arbitrary file at any time
    lReader.SaveHistory (lHistoryBackupFilename);

    // The end-of-input typically corresponds to a CTRL-D typed by the user
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
                << "List airlines, flights and departure dates" << std::endl;
      std::cout << " select" << "\t\t"
                << "Select a flight-date to become the current one"
                << std::endl;
      std::cout << " display" << "\t"
                << "Display the current flight-date" << std::endl;
      std::cout << " sell" << "\t\t"
                << "Make a booking on the current flight-date" << std::endl;
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
      TokenList_T lTokenList = extractTokenListForFlight (lTokenListByReadline);

      stdair::AirlineCode_T lAirlineCode ("all");
      stdair::FlightNumber_T lFlightNumber (0);
      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      parseFlightKey (lTokenList, lAirlineCode, lFlightNumber);

      //
      const std::string lFlightNumberStr = (lFlightNumber ==0)?" (all)":"";
      std::cout << "List of flights for "
                << lAirlineCode << " " << lFlightNumber << lFlightNumberStr
                << std::endl;

      // DEBUG: Display the flight-date
      const std::string& lFlightDateListStr =
        airinvService.list (lAirlineCode, lFlightNumber);

      if (lFlightDateListStr.empty() == false) {
        std::cout << lFlightDateListStr << std::endl;
        STDAIR_LOG_DEBUG (lFlightDateListStr);

      } else {
        std::cerr << "There is no result for "
                  << lAirlineCode << " " << lFlightNumber << lFlightNumberStr
                  << ". Just type the list command without any parameter "
                  << "to see the flight-dates for all the airlines and for all "
                  << "the flight numbers."
                  << std::endl;
      }

      break;
    }

      // ////////////////////////////// Select ////////////////////////
    case Command_T::SELECT: {
      //
      TokenList_T lTokenList =
        extractTokenListForFlightDate (lTokenListByReadline);

      // Check whether the user wants to select the last saved flight-date
      if (lTokenList.empty() == false) {
        // Read the booking class
        TokenList_T::const_iterator itTok = lTokenList.begin();

        if (*itTok == "-") {

          // Swap the current state with the last state
          boost::swap (lInteractiveAirlineCode, lLastInteractiveAirlineCode);
          boost::swap (lInteractiveFlightNumber, lLastInteractiveFlightNumber);
          boost::swap (lInteractiveDate, lLastInteractiveDate);
        
          break;
        }
      }
      
      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      parseFlightDateKey (lTokenList, lInteractiveAirlineCode,
                          lInteractiveFlightNumber, lInteractiveDate);

      // Check whether the selected flight-date is valid
      const bool isFlightDateValid =
        airinvService.check (lInteractiveAirlineCode, lInteractiveFlightNumber,
                             lInteractiveDate);
      if (isFlightDateValid == false) {
        std::ostringstream oFDKStr;
        oFDKStr << "The " << lInteractiveAirlineCode
                << lInteractiveFlightNumber << " / " << lInteractiveDate
                << " flight-date is not valid. Make sure it exists (e.g.,"
                << " with the list command). The current flight-date is kept"
                << " selected.";
        std::cout << oFDKStr.str() << std::endl;
        STDAIR_LOG_ERROR (oFDKStr.str());
        
        // Restore the last state
        lInteractiveAirlineCode = lLastInteractiveAirlineCode;
        lInteractiveFlightNumber = lLastInteractiveFlightNumber;
        lInteractiveDate = lLastInteractiveDate;

        break;
      }

      // DEBUG: Display the flight-date selection
      std::ostringstream oFDKStr;
      oFDKStr << "Selected the " << lInteractiveAirlineCode
              << lInteractiveFlightNumber << " / " << lInteractiveDate
              << " flight-date";
      std::cout << oFDKStr.str() << std::endl;
      STDAIR_LOG_DEBUG (oFDKStr.str());

      // Save the last state
      lLastInteractiveAirlineCode = lInteractiveAirlineCode;
      lLastInteractiveFlightNumber = lInteractiveFlightNumber;
      lLastInteractiveDate = lInteractiveDate;

      break;
    }

      // ////////////////////////////// Display ////////////////////////
    case Command_T::DISPLAY: {
      // DEBUG: Display the flight-date
      const std::string& lCSVFlightDateDump =
        airinvService.csvDisplay (lInteractiveAirlineCode,
                                  lInteractiveFlightNumber, lInteractiveDate);
      std::cout << lCSVFlightDateDump << std::endl;
      STDAIR_LOG_DEBUG (lCSVFlightDateDump);

      break;
    }

      // ////////////////////////////// Sell ////////////////////////
    case Command_T::SELL: {
      //
      TokenList_T lTokenList = extractTokenListForClass (lTokenListByReadline);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      parseBookingClassKey (lTokenList, lInteractiveBookingClass,
                            lInteractivePartySize,
                            lInteractiveOrigin, lInteractiveDestination);

      // DEBUG: Display the flight-date before the sell
      const std::string& lCSVFlightDateDumpBefore =
        airinvService.csvDisplay (lInteractiveAirlineCode,
                                  lInteractiveFlightNumber, lInteractiveDate);
      //std::cout << lCSVFlightDateDumpBefore << std::endl;
      STDAIR_LOG_DEBUG (lCSVFlightDateDumpBefore);

      // Make a booking
      std::ostringstream oSDKStr;
      oSDKStr << lInteractiveAirlineCode << ","
              << lInteractiveFlightNumber << ","
              << lInteractiveDate << ","
              << lInteractiveOrigin << "," << lInteractiveDestination;
      const std::string lSegmentDateKey (oSDKStr.str());

      // Perform the sell
      const bool isSellSuccessful = 
        airinvService.sell (lSegmentDateKey,
                            lInteractiveBookingClass, lInteractivePartySize);

      // DEBUG
      const std::string isSellSuccessfulStr =
        (isSellSuccessful == true)?"Yes":"No";
      std::ostringstream oSaleStr;
      oSaleStr << "Sale ('" << lSegmentDateKey << "', "
               << lInteractiveBookingClass << ": " << lInteractivePartySize
               << ") successful? " << isSellSuccessfulStr;
      std::cout << oSaleStr.str() << std::endl;
      
      // DEBUG
      STDAIR_LOG_DEBUG (oSaleStr.str());

      // DEBUG: Display the flight-date after the sell
      const std::string& lCSVFlightDateDumpAfter =
        airinvService.csvDisplay (lInteractiveAirlineCode,
                                  lInteractiveFlightNumber, lInteractiveDate);
      //std::cout << lCSVFlightDateDumpAfter << std::endl;
      STDAIR_LOG_DEBUG (lCSVFlightDateDumpAfter);

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
      oStr << "That command is not yet understood: '" << lUserInput
           << "' => " << lTokenListByReadline;
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
