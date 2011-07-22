// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// Simfqt
#include <simfqt/SIMFQT_Service.hpp>
#include <simfqt/config/simfqt-paths.hpp>

// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_SIMFQT_DEFAULT_LOG_FILENAME ("simfqt.log");

/** Default name and location for the (CSV) input file. */
const std::string K_SIMFQT_DEFAULT_FARE_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                         "/fare01.csv");

/** Default for the input type. It can be either built-in or provided by an
    input file. That latter must then be given with the -i option. */
const bool K_SIMFQT_DEFAULT_BUILT_IN_INPUT = false;

/** Early return status (so that it can be differentiated from an error). */
const int K_SIMFQT_EARLY_RETURN_STATUS = 99;

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


// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  // State whether the BOM tree should be built-in or parsed from an input file
  bool isBuiltin;
    
  // Fare input filename
  stdair::Filename_T lFareInputFilename;
  
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
    
  // Initialise the Simfqt service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);

  SIMFQT::SIMFQT_Service simfqtService (lLogParams);
    
  // DEBUG
  STDAIR_LOG_DEBUG ("Welcome to Simfqt");

  // Build a default sample list of travel solutions
  stdair::TravelSolutionList_T lTravelSolutionList;
  simfqtService.buildSampleTravelSolutions (lTravelSolutionList);

  // Build a default booking request
  stdair::BookingRequestStruct lBookingRequest =
    simfqtService.buildBookingRequest();

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {

    // Build the default sample BOM tree (filled with fares) for Simfqt
    simfqtService.buildSampleBom();

  } else {
    
    // Build the BOM tree from parsing a fare file
    simfqtService.parseAndLoad (lFareInputFilename);

  }

  // DEBUG: Display the travel solutions
  const std::string& lTSCSVDump =
    simfqtService.csvDisplay (lTravelSolutionList);
  STDAIR_LOG_DEBUG (lTSCSVDump);
  
  // FareQuote the sample list of travel solutions
  simfqtService.quotePrices (lBookingRequest, lTravelSolutionList);

  // DEBUG: Display the whole BOM tree
  const std::string& lBOMCSVDump = simfqtService.csvDisplay();
  STDAIR_LOG_DEBUG ("BOM tree: " << lBOMCSVDump);
  
  // DEBUG: Display the travel solutions
  const std::string& lTSCSVDumpEnd
    = simfqtService.csvDisplay (lTravelSolutionList);
  STDAIR_LOG_DEBUG (lTSCSVDumpEnd);
  
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

