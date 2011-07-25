// STL
#include <sstream>
#include <fstream>
#include <string>
// Boost (Extended STL)
#include <boost/program_options.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// SimCRS
#include <simcrs/SIMCRS_Service.hpp>
#include <simcrs/config/simcrs-paths.hpp>

// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_SIMCRS_DEFAULT_LOG_FILENAME ("simcrs.log");

/** Default name and location for the (CSV) schedule input file. */
const std::string K_SIMCRS_DEFAULT_SCHEDULE_INPUT_FILENAME (STDAIR_SAMPLE_DIR "/schedule01.csv");

/** Default name and location for the (CSV) O&D input file. */
const std::string K_SIMCRS_DEFAULT_OND_INPUT_FILENAME (STDAIR_SAMPLE_DIR "/ond01.csv");

/** Default name and location for the (CSV) fare input file. */
const std::string K_SIMCRS_DEFAULT_FARE_INPUT_FILENAME (STDAIR_SAMPLE_DIR "/fare01.csv");

/** Default name and location for the (CSV) yield input file. */
const std::string K_SIMCRS_DEFAULT_YIELD_INPUT_FILENAME (STDAIR_SAMPLE_DIR "/yieldstore01.csv");
    

/** Default name and location for the Xapian database. */
const std::string K_SIMCRS_DEFAULT_DB_USER ("dsim");
const std::string K_SIMCRS_DEFAULT_DB_PASSWD ("dsim");
const std::string K_SIMCRS_DEFAULT_DB_DBNAME ("sim_dsim");
const std::string K_SIMCRS_DEFAULT_DB_HOST ("localhost");
const std::string K_SIMCRS_DEFAULT_DB_PORT ("3306");

// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/** Early return status (so that it can be differentiated from an error). */
const int K_SIMCRS_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[],
                       stdair::Filename_T& ioScheduleInputFilename,
                       stdair::Filename_T& ioOnDInputFilename,
                       stdair::Filename_T& ioFareInputFilename,
                       stdair::Filename_T& ioYieldInputFilename,
                       stdair::Filename_T& ioLogFilename,
                       std::string& ioDBUser, std::string& ioDBPasswd,
                       std::string& ioDBHost, std::string& ioDBPort,
                       std::string& ioDBDBName) {

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
    ("schedule,s",
     boost::program_options::value< std::string >(&ioScheduleInputFilename)->default_value(K_SIMCRS_DEFAULT_SCHEDULE_INPUT_FILENAME),
     "(CVS) input file for the schedules")
    ("ond,o",
     boost::program_options::value< std::string >(&ioOnDInputFilename)->default_value(K_SIMCRS_DEFAULT_OND_INPUT_FILENAME),
     "(CVS) input file for the O&D definitions")
    ("fare,f",
     boost::program_options::value< std::string >(&ioFareInputFilename)->default_value(K_SIMCRS_DEFAULT_FARE_INPUT_FILENAME),
     "(CVS) input file for the fares")
    ("yield,f",
     boost::program_options::value< std::string >(&ioYieldInputFilename)->default_value(K_SIMCRS_DEFAULT_YIELD_INPUT_FILENAME),
     "(CVS) input file for the yields")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_SIMCRS_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ("user,u",
     boost::program_options::value< std::string >(&ioDBUser)->default_value(K_SIMCRS_DEFAULT_DB_USER),
     "SQL database hostname (e.g., dsim)")
    ("passwd,p",
     boost::program_options::value< std::string >(&ioDBPasswd)->default_value(K_SIMCRS_DEFAULT_DB_PASSWD),
     "SQL database hostname (e.g., dsim)")
    ("host,H",
     boost::program_options::value< std::string >(&ioDBHost)->default_value(K_SIMCRS_DEFAULT_DB_HOST),
     "SQL database hostname (e.g., localhost)")
    ("port,P",
     boost::program_options::value< std::string >(&ioDBPort)->default_value(K_SIMCRS_DEFAULT_DB_PORT),
     "SQL database port (e.g., 3306)")
    ("dbname,m",
     boost::program_options::value< std::string >(&ioDBDBName)->default_value(K_SIMCRS_DEFAULT_DB_DBNAME),
     "SQL database name (e.g., dsim)")
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

  std::ifstream ifs ("simcrs.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_SIMCRS_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_SIMCRS_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_SIMCRS_EARLY_RETURN_STATUS;
  }

  if (vm.count ("schedule")) {
    ioScheduleInputFilename = vm["schedule"].as< std::string >();
    std::cout << "Schedule input filename is: " << ioScheduleInputFilename
              << std::endl;
  }

  if (vm.count ("ond")) {
    ioOnDInputFilename = vm["ond"].as< std::string >();
    std::cout << "O&D input filename is: " << ioOnDInputFilename << std::endl;
  }

  if (vm.count ("fare")) {
    ioFareInputFilename = vm["fare"].as< std::string >();
    std::cout << "Fare input filename is: " << ioFareInputFilename << std::endl;
  }

  if (vm.count ("yield")) {
    ioYieldInputFilename = vm["yield"].as< std::string >();
    std::cout << "Yield input filename is: " << ioYieldInputFilename << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  if (vm.count ("user")) {
    ioDBUser = vm["user"].as< std::string >();
    std::cout << "SQL database user name is: " << ioDBUser << std::endl;
  }

  if (vm.count ("passwd")) {
    ioDBPasswd = vm["passwd"].as< std::string >();
    //std::cout << "SQL database user password is: " << ioDBPasswd << std::endl;
  }

  if (vm.count ("host")) {
    ioDBHost = vm["host"].as< std::string >();
    std::cout << "SQL database host name is: " << ioDBHost << std::endl;
  }

  if (vm.count ("port")) {
    ioDBPort = vm["port"].as< std::string >();
    std::cout << "SQL database port number is: " << ioDBPort << std::endl;
  }

  if (vm.count ("dbname")) {
    ioDBDBName = vm["dbname"].as< std::string >();
    std::cout << "SQL database name is: " << ioDBDBName << std::endl;
  }

  return 0;
}

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // Schedule input filename
  stdair::Filename_T lScheduleInputFilename;
    
  // O&D input filename
  stdair::Filename_T lOnDInputFilename;
    
  // Fare input filename
  stdair::Filename_T lFareInputFilename;
    
  // Yield input filename
  stdair::Filename_T lYieldInputFilename;
    
  // Output log File
  stdair::Filename_T lLogFilename;

  // SQL database parameters
  std::string lDBUser;
  std::string lDBPasswd;
  std::string lDBHost;
  std::string lDBPort;
  std::string lDBDBName;
                       
  // CRS code
  const SIMCRS::CRSCode_T lCRSCode ("1P");
    
  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lScheduleInputFilename, lOnDInputFilename,
                       lFareInputFilename, lYieldInputFilename, lLogFilename,
                       lDBUser, lDBPasswd, lDBHost, lDBPort, lDBDBName);

  if (lOptionParserStatus == K_SIMCRS_EARLY_RETURN_STATUS) {
    return 0;
  }
    
  // Check that the file path given as input corresponds to an actual file
  bool doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lScheduleInputFilename);
  if (doesExistAndIsReadable == false) {
    STDAIR_LOG_ERROR ("The '" << lScheduleInputFilename
                      << "' input file can not be open and read");
    return -1;
  }

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lOnDInputFilename);
  if (doesExistAndIsReadable == false) {
    STDAIR_LOG_ERROR ("The '" << lOnDInputFilename
                      << "' input file can not be open and read");
    return -1;
  }

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lFareInputFilename);
  if (doesExistAndIsReadable == false) {
    STDAIR_LOG_ERROR ("The '" << lFareInputFilename
                      << "' input file can not be open and read");
    return -1;
  }

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lYieldInputFilename);
  if (doesExistAndIsReadable == false) {
    STDAIR_LOG_ERROR ("The '" << lYieldInputFilename
                      << "' input file can not be open and read");
    return -1;
  }

  // Set the database parameters
  const stdair::BasDBParams lDBParams (lDBUser, lDBPasswd, lDBHost, lDBPort,
                                       lDBDBName);
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Initialise the list of classes/buckets
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SIMCRS::SIMCRS_Service simcrsService (lLogParams, lCRSCode,
                                        lScheduleInputFilename,
                                        lOnDInputFilename, lFareInputFilename,
                                        lYieldInputFilename);

  // TODO: instead of building a sample, read the parameters from the
  //       command-line options, and build the corresponding booking request
  const bool isForCRS = true;
  const stdair::BookingRequestStruct& lBookingRequest =
    simcrsService.buildSampleBookingRequest (isForCRS);

  // Calculate the travel solutions corresponding to the given booking request
  stdair::TravelSolutionList_T lTravelSolutionList =
    simcrsService.calculateSegmentPathList (lBookingRequest);
  
  // Check whether everything was fine
  if (lTravelSolutionList.empty() == true) {
    STDAIR_LOG_ERROR ("No travel solution has been found for: "
                      << lBookingRequest.display());
    return -1;
  }

  // Price the travel solution
  simcrsService.fareQuote (lBookingRequest, lTravelSolutionList);

  // Choose a random travel solution: the first one.
  stdair::TravelSolutionStruct& lChosenTravelSolution =
    lTravelSolutionList.front();

  // Get the segment path of the travel solution.
  const stdair::KeyList_T& lsegmentDateKeyList =
    lChosenTravelSolution.getSegmentPath();

  const stdair::FareOptionList_T& lFareOptionList =
    lChosenTravelSolution.getFareOptionList();

  // Check whether everything was fine
  if (lFareOptionList.empty() == true) {
    STDAIR_LOG_ERROR ("No fare option for the chosen travel solution: "
                      << lChosenTravelSolution.display());
    return -1;
  }

  //
  const stdair::FareOptionStruct& lFareOption = lFareOptionList.front();
  lChosenTravelSolution.setChosenFareOption (lFareOption);

  // DEBUG
  const std::string& lSegmentDateKey = lsegmentDateKeyList.front();
  STDAIR_LOG_DEBUG ("The chosen travel solution is: " << lSegmentDateKey
                    << ", the fare is: " << lFareOption.getFare() << " Euros.");

  // Make a booking (reminder: party size is 3)
  const stdair::PartySize_T lPartySize (3);
  const bool isSellSuccessful =
    simcrsService.sell (lChosenTravelSolution, lPartySize);

  // DEBUG
  STDAIR_LOG_DEBUG ("Sale ('" << lBookingRequest << "'): "
                    << " successful? " << isSellSuccessful);

  // DEBUG: Display the whole BOM tree
  const std::string& lCSVDump = simcrsService.csvDisplay();
  STDAIR_LOG_DEBUG (lCSVDump);

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
