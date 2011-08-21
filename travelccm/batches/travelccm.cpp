/*!
 * \page batch_travelccm_cpp Command-Line Utility to Demonstrate Typical TravelCCM Usage
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
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Service.hpp>
#include <travelccm/config/travelccm-paths.hpp>

// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_TRAVELCCM_DEFAULT_LOG_FILENAME ("travelccm.log");

/**
 * Default name and location for the (CSV) input file.
 */
const std::string K_TRAVELCCM_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                      "/ccm_01.csv");

/**
 * Default for the input type. It can be either built-in or provided by an
 * input file. That latter must then be given with the -i option.
 */
const bool K_TRAVELCCM_DEFAULT_BUILT_IN_INPUT = false;

/**
 * Early return status (so that it can be differentiated from an error).
 */
const int K_TRAVELCCM_EARLY_RETURN_STATUS = 99;


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
                       stdair::Filename_T& lInputFilename,
                       stdair::Filename_T& lLogFilename) {
  
  // Default for the built-in input
  ioIsBuiltin = K_TRAVELCCM_DEFAULT_BUILT_IN_INPUT;
    
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
     "The sample BOM tree can be either built-in or parsed from an input file. That latter must then be given with the -i/--input option")
    ("input,i",
     boost::program_options::value< std::string >(&lInputFilename)->default_value(K_TRAVELCCM_DEFAULT_INPUT_FILENAME),
     "(CSV) input file for the customer choice rule sets")
    ("log,l",
     boost::program_options::value< std::string >(&lLogFilename)->default_value(K_TRAVELCCM_DEFAULT_LOG_FILENAME),
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

  std::ifstream ifs ("travelccm.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_TRAVELCCM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_TRAVELCCM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_TRAVELCCM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("builtin")) {
    ioIsBuiltin = true;
  }
  const std::string isBuiltinStr = (ioIsBuiltin == true)?"yes":"no";
  std::cout << "The BOM should be built-in? " << isBuiltinStr << std::endl;

  if (ioIsBuiltin == false) {

    // The BOM tree should be built from parsing a customer-choice rule file
    if (vm.count ("input")) {
      lInputFilename = vm["input"].as< std::string >();
      std::cout << "Input filename is: " << lInputFilename << std::endl;

    } else {
      // The built-in option is not selected. However, no demand input file
      // is specified
      std::cerr << "Either one among the -b/--builtin and -i/--input "
                << "options must be specified" << std::endl;
    }
  }

  if (vm.count ("log")) {
    lLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << lLogFilename << std::endl;
  }
  
  return 0;
}


// ///////// M A I N ////////////
int main (int argc, char* argv[]) {
    
  // State whether the BOM tree should be built-in or parsed from an input file
  bool isBuiltin;

  // Input file name
  stdair::Filename_T lInputFilename;
  
  // Output log File
  stdair::Filename_T lLogFilename;
  
  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, isBuiltin, lInputFilename, lLogFilename);
  
  if (lOptionParserStatus == K_TRAVELCCM_EARLY_RETURN_STATUS) {
    return 0;
  }
  
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the service context
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  
  // Build the BOM tree
  TRAVELCCM::TRAVELCCM_Service travelccmService (lLogParams);

  // DEBUG
  STDAIR_LOG_DEBUG ("Welcome to TravelCCM");

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {
    // Create a sample Customer-Choice rule object, and insert it
    // within the BOM tree
    travelccmService.buildSampleBom();

  } else {
    /**
     * Create the Customer-Choice rule objects, and insert them within
     * the BOM tree.
     *
     * \note For now, there is no input file parser.
     */
    // travelccmService.parseAndLoad (lInputFilename);
  }  

  // Build a list of travel solutions
  const stdair::BookingRequestStruct& lBookingRequest =
    travelccmService.buildSampleBookingRequest();

  // DEBUG
  STDAIR_LOG_DEBUG ("Booking request: " << lBookingRequest.display());

  // Build the sample BOM tree
  stdair::TravelSolutionList_T lTSList;
  travelccmService.buildSampleTravelSolutions (lTSList);

  // DEBUG: Display the list of travel solutions
  const std::string& lCSVDump = travelccmService.csvDisplay (lTSList);
  STDAIR_LOG_DEBUG (lCSVDump);
  
  // Choose a travel solution
  const stdair::TravelSolutionStruct* lTS_ptr =
    travelccmService.chooseTravelSolution (lTSList, lBookingRequest);

  if (lTS_ptr != NULL) {
    // DEBUG
    STDAIR_LOG_DEBUG ("Chosen travel solution: " << lTS_ptr->display());

  } else {
    // DEBUG
    STDAIR_LOG_DEBUG ("No travel solution can be found for "
                      << lBookingRequest.display()
                      << " within the following list of travel solutions");
    STDAIR_LOG_DEBUG (lCSVDump);
  }

  // Close the Log outputFile
  logOutputFile.close();

  /**
   * Note: as that program is not intended to be run on a server in
   * production, it is better not to catch the exceptions. When it
   * happens (that an exception is throwned), that way we get the
   * call stack.
   */

  return 0;	
}
