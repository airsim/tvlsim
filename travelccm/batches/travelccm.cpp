// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/program_options.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Service.hpp>
#include <travelccm/config/travelccm-paths.hpp>

// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_TRAVELCCM_DEFAULT_LOG_FILENAME ("travelccm.log");

/** Default name and location for the (CSV) input file. */
const std::string K_TRAVELCCM_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR "/ccm_01.csv");

/** Default number of random draws to be generated (best if over 100). */
const int K_TRAVELCCM_DEFAULT_RANDOM_DRAWS = 100000;


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
int readConfiguration (int argc, char* argv[], int& lRandomDraws, 
                       std::string& lInputFilename, std::string& lLogFilename) {
  
    
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
    ("draws,d",
     boost::program_options::value<int>(&lRandomDraws)->default_value(K_TRAVELCCM_DEFAULT_RANDOM_DRAWS), 
     "Number of to-be-generated random draws")
    ("input,i",
     boost::program_options::value< std::string >(&lInputFilename)->default_value(K_TRAVELCCM_DEFAULT_INPUT_FILENAME),
     "(CVS) input file for the demand distributions")
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

  std::cout << "The number of random draws is: " << lRandomDraws << std::endl;
  
  return 0;
}


// ///////// M A I N ////////////
int main (int argc, char* argv[]) {
    
  // Number of random draws to be generated (best if greater than 100)
  int lRandomDraws = 0;
  
  // Input file name
  std::string lInputFilename;
  
  // Output log File
  std::string lLogFilename;
  
  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lRandomDraws, lInputFilename, lLogFilename);
  
  if (lOptionParserStatus == K_TRAVELCCM_EARLY_RETURN_STATUS) {
    return 0;
  }
  
  // Check wether or not a (CSV) input file should be read
  bool hasInputFile = false;
  if (lInputFilename.empty() == false) {
    hasInputFile = true;
  }
  
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the service context
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  const TRAVELCCM::TravelCCMType lCCMType(TRAVELCCM::TravelCCMType::PREF_BASED);
  TRAVELCCM::TRAVELCCM_Service travelccmService (lLogParams, lCCMType);
  
  // Start a mini-simulation
  travelccmService.simulate();
  
  return 0;	
}
