// STL
#include <sstream>
#include <fstream>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
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
                       std::string& ioLogFilename,
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
  std::string lScheduleInputFilename;
    
  // O&D input filename
  std::string lOnDInputFilename;
    
  // Fare input filename
  std::string lFareInputFilename;
    
  // Output log File
  std::string lLogFilename;

  // SQL database parameters
  std::string lDBUser;
  std::string lDBPasswd;
  std::string lDBHost;
  std::string lDBPort;
  std::string lDBDBName;
                       
  // CRS code
  std::string lCRSCode ("1P");
    
  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lScheduleInputFilename, lOnDInputFilename,
                       lFareInputFilename, lLogFilename,
                       lDBUser, lDBPasswd, lDBHost, lDBPort, lDBDBName);

  if (lOptionParserStatus == K_SIMCRS_EARLY_RETURN_STATUS) {
    return 0;
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
                                        lOnDInputFilename,
                                        lFareInputFilename);

  // Create an empty booking request structure
  // TODO: fill the booking request structure from the input parameters
  const stdair::AirportCode_T lOrigin ("NCE");
  const stdair::AirportCode_T lDestination ("BOS");
  const stdair::AirportCode_T lPOS ("NYC");
  const stdair::Date_T lPreferredDepartureDate(2011, boost::gregorian::Jan, 20);
  const stdair::Date_T lRequestDate (2011, boost::gregorian::Jan, 12);
  const stdair::Duration_T lRequestTime (boost::posix_time::hours(10));
  const stdair::DateTime_T lRequestDateTime (lRequestDate, lRequestTime);
  const stdair::CabinCode_T lPreferredCabin ("Eco");
  const stdair::NbOfSeats_T lPartySize (1);
  const stdair::ChannelLabel_T lChannel ("D");
  const stdair::TripType_T lTripType ("RI");
  const stdair::DayDuration_T lStayDuration (7);
  const stdair::FrequentFlyer_T lFrequentFlyerType ("M");
  const stdair::Duration_T lPreferredDepartureTime (boost::posix_time::hours(10));
  const stdair::WTP_T lWTP (1000.0);
  const stdair::PriceValue_T lValueOfTime (100.0);
  const stdair::BookingRequestStruct lBookingRequest (lOrigin, lDestination,
                                                      lPOS,
                                                      lPreferredDepartureDate,
                                                      lRequestDateTime,
                                                      lPreferredCabin,
                                                      lPartySize, lChannel,
                                                      lTripType, lStayDuration,
                                                      lFrequentFlyerType,
                                                      lPreferredDepartureTime,
                                                      lWTP, lValueOfTime);
  const stdair::SegmentPathList_T lSegmentPath;
  
  // Price the travel solution
  stdair::TravelSolutionList_T lTravelSolutionList =
    simcrsService.getFareQuote (lBookingRequest, lSegmentPath);

  // DEBUG
  const stdair::TravelSolutionStruct& lTravelSolution =
    lTravelSolutionList.front();
  STDAIR_LOG_DEBUG ("The price given by the fare quoter is: "
                    << lTravelSolution.getFare() << " Euros");

  // Make a booking
  // const std::string lAirlineCode ("SV");
  // const stdair::PartySize_T lPartySize = 5;
  // simcrsService.sell (lAirlineCode, lPartySize);

  return 0;
}
