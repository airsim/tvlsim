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
/** Default name and location for the log file. */
const std::string K_TRAVELCCM_DEFAULT_LOG_FILENAME ("travelccm.log");

/** Default name and location for the (CSV) input file. */
const std::string K_TRAVELCCM_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                      "/ccm_01.csv");


// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/** Early return status (so that it can be differentiated from an error). */
const int K_TRAVELCCM_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], std::string& lInputFilename,
                       std::string& lLogFilename) {
  
    
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
    ("input,i",
     boost::program_options::value< std::string >(&lInputFilename)->default_value(K_TRAVELCCM_DEFAULT_INPUT_FILENAME),
     "(CVS) input file for customer choice")
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

  if (vm.count ("input")) {
    lInputFilename = vm["input"].as< std::string >();
    std::cout << "Input filename is: " << lInputFilename << std::endl;
  }

  if (vm.count ("log")) {
    lLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << lLogFilename << std::endl;
  }
  
  return 0;
}


// ///////// M A I N ////////////
int main (int argc, char* argv[]) {
    
  // Input file name
  std::string lInputFilename;
  
  // Output log File
  std::string lLogFilename;
  
  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lInputFilename, lLogFilename);
  
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

  /*
    Note: as that program is not intended to be run on a server in
    production, it is better not to catch the exceptions. When it
    happens (that an exception is throwned), that way we get the
    call stack.
  */

  return 0;	
}
