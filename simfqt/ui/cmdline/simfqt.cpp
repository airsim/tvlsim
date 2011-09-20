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
#include <stdair/basic/BasConst_BomDisplay.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasConst_DefaultObject.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/ParsedKey.hpp>
#include <stdair/bom/BomKeyManager.hpp>
#include <stdair/command/CmdBomManager.hpp>
// Stdair GNU Readline Wrapper
#include <stdair/ui/cmdline/SReadline.hpp>
// Simfqt
#include <simfqt/SIMFQT_Service.hpp>
#include <simfqt/config/simfqt-paths.hpp>


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_SIMFQT_DEFAULT_LOG_FILENAME ("simfqt.log");

/**
 * Default name and location for the (CSV) input file.
 */
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

/**
 * Read and parse the command line options.
 */
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

  std::ifstream ifs ("simfqt.cfg");
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
  Completers.push_back ("price %airline_code %flight_number %departure_date %airport_code %airport_code %departure_time %booking_date %booking_time %POS %channel% %trip_type %stay_duration");
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
// Re-compose a date using three strings: the year, the month and the
// day. Return true if a correct date has been computed, false if not.
bool retrieveDate (std::string iYearString,
                   std::string iMonthString,
                   std::string iDayString,
                   stdair::Date_T& ioDate) {
  
  const std::string kMonthStr[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  
  // Check the year.
  unsigned short lDateYear;
  try {

    lDateYear = boost::lexical_cast<unsigned short> (iYearString);
    if (lDateYear < 100) {
      lDateYear += 2000;
    }
      
  } catch (boost::bad_lexical_cast& eCast) {
    std::cerr << "The year ('" << iYearString
              << "') cannot be understood." << std::endl;
    return false;
  }

  // Check the month.
  std::string lDateMonthStr;
  try {

    const boost::regex lMonthRegex ("^(\\d{1,2})$");
    const bool isMonthANumber = regex_match (iMonthString, lMonthRegex);

    if (isMonthANumber == true) {
      const unsigned short lMonth =
        boost::lexical_cast<unsigned short> (iMonthString);
      if (lMonth > 12) {
        throw boost::bad_lexical_cast();
      }
      if (lMonth != 0) {
        lDateMonthStr = kMonthStr[lMonth-1];
      } else {
        std::cerr << "The month ('" << iMonthString
              << "') cannot be understood." << std::endl;
        return false;
      }
      
    } else {
      if (iMonthString.size() < 3) {
        throw boost::bad_lexical_cast();
      }
      std::string lMonthStr1 (iMonthString.substr (0, 1));
      boost::algorithm::to_upper (lMonthStr1);
      std::string lMonthStr23 (iMonthString.substr (1, 2));
      boost::algorithm::to_lower (lMonthStr23);
      lDateMonthStr = lMonthStr1 + lMonthStr23;
    }

  } catch (boost::bad_lexical_cast& eCast) {
    std::cerr << "The month ('" << iMonthString
              << "') cannot be understood." << std::endl;
    return false;
  }

  // Check the day.
  unsigned short lDateDay;
  try {

    lDateDay = boost::lexical_cast<unsigned short> (iDayString);

  } catch (boost::bad_lexical_cast& eCast) {
    std::cerr << "The day ('" << iDayString
              << "') cannot be understood." << std::endl;
    return false;
  }

  // Re-compose the date.
  std::ostringstream lDateStr;
  lDateStr << lDateYear << "-" << lDateMonthStr
           << "-" << lDateDay;
  try {

    ioDate =
      boost::gregorian::from_simple_string (lDateStr.str());

  } catch (boost::gregorian::bad_month& eCast) {
    std::cerr << "The month of the date ('" << lDateStr.str()
              << "') cannot be understood." << std::endl;
    return false;
  } catch (boost::gregorian::bad_day_of_month& eCast) {
    std::cerr << "The date ('" << lDateStr.str()
              << "') is not correct: the day of month does not exist."
              << std::endl;
    return false;
  } catch (boost::gregorian::bad_year& eCast) {
    std::cerr << "The year ('" << lDateStr.str()
              << "') is not correct."
              << std::endl;
    return false;
  }
  
  return true;
}

// //////////////////////////////////////////////////////////////////
// Re-compose a time using two strings: the hour and the minute.
// Return true if a correct time has been computed, false if not.
bool retrieveTime (std::string iHourString,
                   std::string iMinuteString,
                   stdair::Duration_T& oTime) {

  // Check the hour
  unsigned short lTimeHour;
  try {

    lTimeHour = boost::lexical_cast<unsigned short> (iHourString);
      
  } catch (boost::bad_lexical_cast& eCast) {
    std::cerr << "The hour of the time ('" << iHourString
              << "') cannot be understood." << std::endl;
    return false;
  }
  
  // Check the minutes
  unsigned short lTimeMinute;
  try {
        
    lTimeMinute = boost::lexical_cast<unsigned short> (iMinuteString);
    
  } catch (boost::bad_lexical_cast& eCast) {
    std::cerr << "The minute of the time ('" << iMinuteString
              << "') cannot be understood." << std::endl;
    return false;
  }


  // Re-compose the time
  std::ostringstream lTimeStr;
  lTimeStr << lTimeHour << ":" << lTimeMinute;
  oTime =
    boost::posix_time::duration_from_string (lTimeStr.str());

  return true;
}

// //////////////////////////////////////////////////////////////////
// Analyze the tokens of the 'price' command in order to construct
// a travel solution list and a booking request.
const stdair::BookingRequestStruct parseTravelSolutionAndBookingRequestKey
(const TokenList_T& iTokenList,
 stdair::TravelSolutionList_T& ioInteractiveTravelSolutionList,
 const stdair::BookingRequestStruct& ioBookingRequestStruct) {

  TokenList_T::const_iterator itTok = iTokenList.begin();
  
  if (itTok->empty() == true) {
    
    std::cerr << "Wrong list of parameters. "
              << "The default booking request and travel solution list are kept."
              << std::endl;
    return ioBookingRequestStruct;
    
    
  } else {
    // Parameters corresponding to the tokens.
    // Each parameter correponds to one token except the dates
    // (three tokens) and the times (two tokens). 
    stdair::AirlineCode_T lAirlineCode;
    stdair::FlightNumber_T lflightNumber;
    stdair::Date_T lDepartureDate;
    stdair::Duration_T lDepartureTime;
    stdair::AirportCode_T lOriginAirport;
    stdair::AirportCode_T lDestinationAirport;
    stdair::Date_T lRequestDate;
    stdair::Duration_T lRequestTime;
    stdair::CityCode_T lPOS;
    stdair::ChannelLabel_T lChannel;
    stdair::TripType_T lTripType;
    unsigned short lStayDuration;

    // Read the airline code.
    lAirlineCode = *itTok;
    boost::algorithm::to_upper (lAirlineCode);

    // Read the flight-number  .
    ++itTok;
    if (itTok->empty() == false) {
      try {

        lflightNumber = boost::lexical_cast<stdair::FlightNumber_T> (*itTok);

      } catch (boost::bad_lexical_cast& eCast) {
        std::cerr << "The flight number ('" << *itTok
                  << "') cannot be understood."
                  << std::endl;
        return ioBookingRequestStruct;
      }
    }
    
    // Read the departure date.
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lDepartureYearString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lDepartureMonthString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lDepartureDayString = *itTok;
    const bool IsDepartureDateReadable =
      retrieveDate (lDepartureYearString, lDepartureMonthString,
                    lDepartureDayString, lDepartureDate);
    
    if (IsDepartureDateReadable == false) {
      std::cerr << "The default booking request and travel solution list are kept."
                << std::endl;
      return ioBookingRequestStruct;
    }

    // Read the origin.
    ++itTok;
    if (itTok->empty() == false) {
      lOriginAirport = *itTok;
      boost::algorithm::to_upper (lOriginAirport);
    } 

    // Read the destination.
    ++itTok;
    if (itTok->empty() == false) {
      lDestinationAirport = *itTok;
      boost::algorithm::to_upper (lDestinationAirport);
    }

    // Read the departure time.
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lDepartureHourString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lDepartureMinuteString = *itTok;
    const bool IsDepartureTimeReadable =
      retrieveTime (lDepartureHourString, lDepartureMinuteString,
                    lDepartureTime);
    
    if (IsDepartureTimeReadable == false) {
      std::cerr << "The default booking request and travel solution list are kept."
                << std::endl;
      return ioBookingRequestStruct;
    }

    // Read the request date.
    ++itTok;
    if (itTok->empty() == true) {
        return ioBookingRequestStruct;
    }
    const std::string lRequestYearString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lRequestMonthString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lRequestDayString = *itTok;
    const bool IsRequestDateReadable =
      retrieveDate (lRequestYearString, lRequestMonthString,
                    lRequestDayString, lRequestDate);
    
    if (IsRequestDateReadable == false) {
      std::cerr << "The default booking request and travel solution list are kept."
                << std::endl;
      return ioBookingRequestStruct;
    }

    // Read the request time.
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lRequestHourString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return ioBookingRequestStruct;
    }
    const std::string lRequestMinuteString = *itTok;
    const bool IsRequestTimeReadable =
      retrieveTime (lRequestHourString, lRequestMinuteString,
                    lRequestTime);
    
    if (IsRequestTimeReadable == false) {
      std::cerr << "The default booking request and travel solution list are kept."
                << std::endl;
      return ioBookingRequestStruct;
    }
    
    // Read the POS.
    ++itTok;
    if (itTok->empty() == false) {
      lPOS = *itTok;
      boost::algorithm::to_upper (lPOS);
    }

    // Read the channel.
    ++itTok;
    if (itTok->empty() == false) {
      lChannel = *itTok;
      boost::algorithm::to_upper (lChannel);
    }

    // Read the trip type.
    ++itTok;
    if (itTok->empty() == false) {
      lTripType = *itTok;
      boost::algorithm::to_upper (lTripType);
    }

    // Read the stay duration.
    ++itTok;
    if (itTok->empty() == false) {
      try {

        lStayDuration = boost::lexical_cast<unsigned short> (*itTok);

      } catch (boost::bad_lexical_cast& eCast) {
        std::cerr << "The stay duration ('" << *itTok
                  << "') cannot be understood." << std::endl;
        return ioBookingRequestStruct;
      }
    }  

    // At this step we know that all the parameters designed to construct
    // the travel solution and the booking request are correct.

    // Empty the travel solution list to store a new travel solution.
    ioInteractiveTravelSolutionList.pop_front();
    // Construct the new travel solution. 
    stdair::TravelSolutionStruct lTravelSolution;
    std::ostringstream oStr;
    oStr << lAirlineCode
         << stdair::DEFAULT_KEY_FLD_DELIMITER
         << lflightNumber
         << stdair::DEFAULT_KEY_SUB_FLD_DELIMITER
         << lDepartureDate
         << stdair::DEFAULT_KEY_FLD_DELIMITER
         << lOriginAirport
         << stdair::DEFAULT_KEY_SUB_FLD_DELIMITER
         << lDestinationAirport
         << stdair::DEFAULT_KEY_FLD_DELIMITER
         << lDepartureTime;
    lTravelSolution.addSegment (oStr.str());
    ioInteractiveTravelSolutionList.push_front(lTravelSolution);

    // Construct the new booking request.
    stdair::DateTime_T lRequestDateTime (lRequestDate, lRequestTime);
    const stdair::BookingRequestStruct &lBookingRequestStruct =
      stdair::BookingRequestStruct(lOriginAirport,
                                   lDestinationAirport,
                                   lPOS,
                                   lDepartureDate,
                                   lRequestDateTime,
                                   stdair::CABIN_ECO,
                                   stdair::DEFAULT_PARTY_SIZE,
                                   lChannel,
                                   lTripType,
                                   lStayDuration,
                                   stdair::FREQUENT_FLYER_MEMBER,
                                   lDepartureTime,
                                   stdair::DEFAULT_WTP,
                                   stdair::DEFAULT_VALUE_OF_TIME);

    return lBookingRequestStruct;
  }
}

// //////////////////////////////////////////////////////////////////
// Analyze the tokens of the 'display' command in order to retrieve
// an airport pair and a departure date.
void parseFlightDateKey (const TokenList_T& iTokenList,
                         stdair::AirportCode_T& ioOrigin,
                         stdair::AirportCode_T& ioDestination,
                         stdair::Date_T& ioDepartureDate) {

  TokenList_T::const_iterator itTok = iTokenList.begin();
  
  // Interpret the user input.
  if (itTok->empty() == true) {

    std::cerr << "Wrong parameters specified. Default paramaters '" 
              << ioOrigin << "-" << ioDestination
              << "/" << ioDepartureDate
              << "' are kept."
             << std::endl;

  } else {
    
    // Read the origin.
    ioOrigin = *itTok;
    boost::algorithm::to_upper (ioOrigin);

    // Read the destination.
    ++itTok;
    if (itTok->empty() == false) {
      ioDestination  = *itTok;
      boost::algorithm::to_upper (ioDestination);
    }

    // Read the departure date.
    ++itTok;
    if (itTok->empty() == true) {
      return;
    }
    std::string lYearString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return;
    }
    std::string lMonthString = *itTok;
    ++itTok;
    if (itTok->empty() == true) {
      return;
    }
    std::string lDayString = *itTok;
    const bool IsDepartureDateReadable =
      retrieveDate (lYearString, lMonthString, lDayString,
                    ioDepartureDate);
    if (IsDepartureDateReadable == false) {
      std::cerr << "Default paramaters '" 
                << ioOrigin << "-" << ioDestination
                << "/" << ioDepartureDate
                << "' are kept." 
                << std::endl;
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
// Parse the token list of the 'price' command.
TokenList_T extractTokenListForTSAndBR (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:            airline_code flight_number departure_date airport_code airport_code departure_time booking_date booking_time POS channel trip_type stay_duration
   *   airline_code:    word (alpha{2,3})
   *   flight_number:   number (digit{1,4})
   *   departure date:  year[/- ]?month[/- ]?day
   *   year:            number (digit{2,4})
   *   month:           (number (digit{1,2}) | word (alpha{3}))
   *   day:             number (digit{1,2})
   *   departure time:  hour[:]?minute
   *   hour:            number (digit{1,2})
   *   minute:          number (digit{1,2})
   *   airport_code:    word (alpha{3})
   *   airport_code:    word (alpha{3})
   *   booking date:    year[/- ]?month[/- ]?day
   *   booking time:    hour[:]?minute
   *   POS:             word (alpha{3})
   *   channel:         word (alpha{2})
   *   trip type:       word (alpha{2})
   *   stay duration    number (digit{1})
   */
  const std::string lRegEx("^([[:alpha:]]{2,3})"
                           "[[:space:]]+([[:digit:]]{1,4})"
                           "[/ ]*"
                           "[[:space:]]+([[:digit:]]{2,4})[/-]?"
                           "[[:space:]]*([[:alpha:]]{3}|[[:digit:]]{1,2})[/-]?"
                           "[[:space:]]*([[:digit:]]{1,2})[[:space:]]*"
                           "[[:space:]]+([[:alpha:]]{3})"
                           "[[:space:]]+([[:alpha:]]{3})"
                           "[[:space:]]+([[:digit:]]{1,2})[:]?([[:digit:]]{1,2})"
                           "[[:space:]]+([[:digit:]]{2,4})[/-]?"
                           "[[:space:]]*([[:alpha:]]{3}|[[:digit:]]{1,2})[/-]?"
                           "[[:space:]]*([[:digit:]]{1,2})"
                           "[[:space:]]+([[:digit:]]{1,2})[:]?([[:digit:]]{1,2})"
                           "[[:space:]]+([[:alpha:]]{3})"
                           "[[:space:]]+([[:alpha:]]{2})"
                           "[[:space:]]+([[:alpha:]]{2})"
                           "[[:space:]]+([[:digit:]]{1})$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
} 

// /////////////////////////////////////////////////////////
// Parse the token list of the 'display' command.
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
  const std::string lRegEx("^([[:alpha:]]{3})"
                           "[[:space:]]*[/-]?"
                           "[[:space:]]*([[:alpha:]]{3})"
                           "[[:space:]]*[/-]?"
                           "[[:space:]]*([[:digit:]]{2,4})"
                           "[[:space:]]*[/-]?"
                           "[[:space:]]*([[:alpha:]]{3}|[[:digit:]]{1,2})"
                           "[[:space:]]*[/-]?"
                           "[[:space:]]*([[:digit:]]{1,2})$");

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
  const std::string lHistoryFilename ("simfqt.hist");
  const std::string lHistoryBackupFilename ("simfqt.hist.bak");

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
  STDAIR_LOG_DEBUG ("Welcome to SimFQT display");

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {
    // Build the sample BOM tree (filled with fares) for Simfqt
    simfqtService.buildSampleBom();
  } else {
    // Build the BOM tree from parsing a fare file
    SIMFQT::FareFilePath lFareFilePath (lFareInputFilename);
    simfqtService.parseAndLoad (lFareFilePath);
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

    // Update the default booking request.
    // If there is an input file, we want the CRS booking request (defined in stdair).
    // If not, we want the default booking request.
    const bool isCRSBookingRequest = !isBuiltin;
    const stdair::BookingRequestStruct& lInteractiveBookingRequest =
      simfqtService.buildBookingRequest (isCRSBookingRequest);

    // Update the default parameters for the following interactive session.
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
      
      // Add the segment date key to the travel solution.
      lInteractiveTravelSolution.addSegment (lBA9_SegmentDateKey);

      // Add the travel solution to the list
      lInteractiveTravelSolutionList.push_back (lInteractiveTravelSolution);
    }
    
    // Prompt.
    std::ostringstream oPromptStr;
    oPromptStr << "simfqt "
               << "> ";
    // The last parameter could be ommited.
    TokenList_T lTokenListByReadline;
    lUserInput = lReader.GetLine (oPromptStr.str(), lTokenListByReadline,
                                  EndOfInput);

    // The history could be saved to an arbitrary file at any time.
    lReader.SaveHistory (lHistoryBackupFilename);

    if (EndOfInput) {
      std::cout << std::endl;
      break;
    }

    // Interpret the user input.
    lCommandType = extractCommand (lTokenListByReadline);
    
    switch (lCommandType) {

      // ////////////////////////////// Help ////////////////////////
    case Command_T::HELP: {
      // Search for information to display default parameters lists.
      // Get the first travel solution.
      stdair::TravelSolutionStruct& lTravelSolutionStruct =
        lInteractiveTravelSolutionList.front();  
      // Get the segment-path of the first travel solution.
      const stdair::SegmentPath_T& lSegmentPath =
        lTravelSolutionStruct.getSegmentPath();  
      // Get the first segment of the first travel solution.
      const std::string& lSegmentDateKey = lSegmentPath.front();
      // Get the parsed key of the first segment of the first travel solution.
      const stdair::ParsedKey& lParsedKey =
        stdair::BomKeyManager::extractKeys (lSegmentDateKey);
      // Get the request date time
      const stdair::DateTime_T& lRequestDateTime =
        lInteractiveBookingRequest.getRequestDateTime();
      const stdair::Time_T lRequestTime =
        lRequestDateTime.time_of_day();
      std::cout << std::endl;
      // Display help.
      std::cout << "Commands: " << std::endl;
      std::cout << " help" << "\t\t" << "Display this help" << std::endl;
      std::cout << " quit" << "\t\t" << "Quit the application" << std::endl;
      std::cout << " list" << "\t\t"
                << "List all the fare rule O&Ds and the corresponding date ranges" << std::endl;
      std::cout << " display" << "\t"
                << "Display all fare rules for an O&D and a departure date. \n" << "\t\t"
                << "If no parameters specified or wrong list of parameters, default values are used: \n"<< "\t\t"
                << "      display " <<  lInteractiveOrigin << " "
                << lInteractiveDestination << " "
                << lInteractiveDepartureDate << std::endl;
      std::cout << " price" << "\t\t"
                << "Price the travel solution corresponding to a booking request. \n" << "\t\t"
                << "If no parameters specified or wrong list of parameters, default value are used: \n" << "\t\t"
                << "      price "
                << lParsedKey._airlineCode << " "
                << lParsedKey._flightNumber << " "
                << lParsedKey._departureDate << " "
                << lParsedKey._boardingPoint << " "
                << lParsedKey._offPoint << " "
                << lParsedKey._boardingTime << " "
                << lRequestDateTime.date() << " "
                << lRequestTime.hours() << ":" << lRequestTime.minutes() << " " 
                << lInteractiveBookingRequest.getPOS() << " "
                << lInteractiveBookingRequest.getBookingChannel() << " " 
                << lInteractiveBookingRequest.getTripType() << " "
                << lInteractiveBookingRequest.getStayDuration() << std::endl;
      std::cout << std::endl;
      break;
    }
      
      // ////////////////////////////// Quit ////////////////////////
    case Command_T::QUIT: {
      break;
    }

      // ////////////////////////////// List /////////////////////////
    case Command_T::LIST: {
      
      // Get the list of all airport pairs and date ranges for which
      // there are fares available.
      const std::string& lAirportPairDateListStr =
        simfqtService.list ();

      if (lAirportPairDateListStr.empty() == false) {
        std::cout << lAirportPairDateListStr << std::endl;
        STDAIR_LOG_DEBUG (lAirportPairDateListStr);

      } else {
        std::cerr << "There is no result for airport pairs and date ranges."
                  << "Make sure your input file is not empty."
                  << std::endl;
      }

      break;
    }

      // ////////////////////////////// Display /////////////////////////
    case Command_T::DISPLAY: {

      // If no parameters are entered by the user, keep default ones.
      if (lTokenListByReadline.empty() == true) {

        std::cout << "No parameters specified. Default paramaters '" 
                  << lInteractiveOrigin << "-" << lInteractiveDestination
                  << "/" << lInteractiveDepartureDate
                  << "' are kept."
                  << std::endl;

      } else {

        // Find the best match corresponding to the given parameters.
        TokenList_T lTokenList =
          extractTokenListForOriDestDate (lTokenListByReadline);
        
        // Parse the best match, and give default values in case the
        // user does not specify all the parameters or does not
        // specify some of them correctly.
        parseFlightDateKey (lTokenList, lInteractiveOrigin,
                            lInteractiveDestination, lInteractiveDepartureDate);
        
      }

      // Check whether the selected airportpair-date is valid:
      // i.e. if there are corresponding fare rules.
      const bool isAirportPairDateValid =
        simfqtService.check (lInteractiveOrigin, lInteractiveDestination,
                             lInteractiveDepartureDate);

      if (isAirportPairDateValid == false) {
        std::ostringstream oFDKStr;
        oFDKStr << "The airport pair/departure date: "
                << lInteractiveOrigin << "-" << lInteractiveDestination
                << "/" << lInteractiveDepartureDate
                << " does not correpond to any fare rule.\n"
                << "Make sure it exists with the 'list' command.";
        std::cout << oFDKStr.str() << std::endl;
        STDAIR_LOG_ERROR (oFDKStr.str());

        break;
      }
      
      // Display the list of corresponding fare rules.
      std::cout << "List of fare rules for "
                << lInteractiveOrigin << "-"
                << lInteractiveDestination << "/"
                << lInteractiveDepartureDate 
                << std::endl;

      const std::string& lFareRuleListStr =
        simfqtService.csvDisplay (lInteractiveOrigin,
                                  lInteractiveDestination,
                                  lInteractiveDepartureDate);

      assert (lFareRuleListStr.empty() == false);
      std::cout << lFareRuleListStr << std::endl;
      STDAIR_LOG_DEBUG (lFareRuleListStr);

      break;
    }
      
      // ////////////////////////////// Price ////////////////////////
    case Command_T::PRICE: {

      // If no parameters are entered by the user, keep default ones.
      if (lTokenListByReadline.empty() == true) {

        lInteractiveTravelSolution = lInteractiveTravelSolutionList.front();
        
        std::cout << "No parameters specified. Default booking request and default travel solution list are kept.\n"
                  << "Booking request: << "
                  << lInteractiveBookingRequest.display()  << " >>"
                  << "\nTravel Solution: << "
                  << lInteractiveTravelSolution.display() << " >>"
                  << "\n********** \n"
                  << "Fare quote"
                  << "\n**********"
                << std::endl;
        
        // Try to fareQuote the sample list of travel solutions.
        try {
        simfqtService.quotePrices (lInteractiveBookingRequest,
                                   lInteractiveTravelSolutionList);
        } catch (stdair::ObjectNotFoundException& E) {
          std::cerr << "The given travel solution corresponding to the given booking request can not be priced.\n"
                    << E.what()
                    << std::endl;
          break;
        }
      } else {
      
        // Find the best match corresponding to the given parameters.
        TokenList_T lTokenList =
          extractTokenListForTSAndBR (lTokenListByReadline);

        // Parse the best match, and give default values in case the
        // user does not specify all the parameters or does not
        // specify some of them correctly.
        stdair::BookingRequestStruct lFinalBookingRequest
          = parseTravelSolutionAndBookingRequestKey (lTokenList,
                                                     lInteractiveTravelSolutionList,
                                                     lInteractiveBookingRequest);

        
        assert (lInteractiveTravelSolutionList.size() >= 1);
        lInteractiveTravelSolution = lInteractiveTravelSolutionList.front();

        // Display the booking request and the first travel solution
        // before pricing.
        std::cout << "Booking request: << "
                  << lFinalBookingRequest.display()  << " >>"
                  << "\nTravel Solution: << "
                  << lInteractiveTravelSolution.display() << " >>"
                  << "\n********** \n"
                  << "Fare quote"
                  << "\n**********"
                << std::endl;
        
        // Try to fareQuote the sample list of travel solutions.
        try {
          simfqtService.quotePrices (lFinalBookingRequest,
                                     lInteractiveTravelSolutionList);
        } catch (stdair::ObjectNotFoundException& E) {
          std::cerr << "The given travel solution corresponding to the given booking request can not be priced.\n"
                    << E.what()
                    << std::endl;
          break;
        }
      }

      // Display the first travel solution after pricing:
      // one or more fare option have been added.
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
      oStr << "The '" << lUserInput << "' command is not yet understood.\n"
           << "Type help to have more information." << std::endl;

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
