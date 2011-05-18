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
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/stdair_date_time_types.hpp>
// Stdair GNU Readline Wrapper
#include <stdair/ui/cmdline/SReadline.hpp>
// Simfqt
#include <simfqt/SIMFQT_Service.hpp>
#include <simfqt/config/simfqt-paths.hpp>


// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_SIMFQT_DEFAULT_LOG_FILENAME ("fareQuote.log");

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
    DISPLAY,
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
     "(CVS) input file for the fare rules")
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
  Completers.push_back ("display");
  Completers.push_back ("quit");

  
  // Now register the completers.
  // Actually it is possible to re-register another set at any time
  ioInputReader.RegisterCompletions (Completers);
}

// //////////////////////////////////////////////////////////////////
Command_T::Type_T extractCommand (const TokenList_T& iTokenList) {
  Command_T::Type_T oCommandType = Command_T::LAST_VALUE;

  // Interpret the user input
  if (iTokenList.empty() == false) {
    TokenList_T::const_iterator itTok = iTokenList.begin();
    const std::string& lCommand (*itTok);
    
    if (lCommand == "display") {
      oCommandType = Command_T::DISPLAY;

    } else if (lCommand == "quit") {
      oCommandType = Command_T::QUIT;
    }

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
  // Interpret the user input
  if (iTokenList.empty() == false) {
    TokenList_T::const_iterator itTok = iTokenList.begin();
    const std::string& lCommand (*itTok);
    assert (lCommand == "display");

    // Read the origin
    ++itTok;
    if (itTok != iTokenList.end()) {
      ioOrigin = *itTok;

    } else {
      return;
    }

    // Read the destination
    ++itTok;
    if (itTok != iTokenList.end()) {
      ioDestination = *itTok;

    } else {
      return;
    }

    // Read the departure date
    ++itTok;
    if (itTok != iTokenList.end()) {
      try {

      ioDepartureDate = boost::gregorian::from_simple_string (*itTok);

      } catch (boost::bad_lexical_cast& eCast) {
        std::cerr << "The flight departure date ('" << *itTok
                  << "') cannot be understood. The default value ("
                  << ioDepartureDate << ") is kept. " << std::endl;
        return;
      }

    } else {
      return;
    }
  }
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
  
  while (lUserInput != "quit" && EndOfInput == false) {
    // The last parameter could be ommited
    TokenList_T lTokenList;
    lUserInput = lReader.GetLine ("fareQuote> ", lTokenList, EndOfInput);

    // The history could be saved to an arbitrary file at any time
    lReader.SaveHistory (lHistoryBackupFilename);

    if (EndOfInput) {
      // DEBUG
      STDAIR_LOG_DEBUG ("End of the session. Exiting.");
      std::cout << "End of the session. Exiting." << std::endl;

      break;
    }

    // Interpret the user input
    const Command_T::Type_T lCommandType = extractCommand (lTokenList);
    switch (lCommandType) {
    case Command_T::DISPLAY: {
      stdair::AirportCode_T lOrigin ("SIN");
      stdair::AirportCode_T lDestination ("BKK");
      stdair::Date_T lDate (2010, 01, 15);
      parseFlightDateKey (lTokenList, lOrigin, lDestination, lDate);

      // DEBUG: Display the fare rule
      const std::string& lCSVFlightDateDump =
        simfqtService.csvDisplay (lOrigin, lDestination, lDate);
      std::cout << lCSVFlightDateDump << std::endl;
      STDAIR_LOG_DEBUG (lCSVFlightDateDump);

      break;
    }

    case Command_T::NOP: {
      break;
    }
 
    case Command_T::QUIT: {
      break;
    }

    case Command_T::LAST_VALUE:
    default: {
      // DEBUG
      std::ostringstream oStr;
      oStr << "That command is not yet understood: '" << lUserInput << "' => ";

      unsigned short idx (0);
      for (TokenList_T::const_iterator itTok = lTokenList.begin();
           itTok != lTokenList.end(); ++itTok, ++idx) {
        if (idx != 0) {
          oStr << ", ";
        }
        const std::string& lToken = *itTok;
        oStr << lToken;
      }

      STDAIR_LOG_DEBUG (oStr.str());
      std::cout << oStr.str() << std::endl;
    }
    }
  }

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
