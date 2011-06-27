/*!
 * \page AirInvServer.cpp Server to Demonstrate Typical AirInv Usage
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>
// Boost (Extended STL)
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
// ZeroMQ
#include <zmq.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/BomJSONImport.hpp>
#include <stdair/bom/BomJSONExport.hpp>
#include <stdair/service/Logger.hpp>
// AirInvServer
#include <airinv/config/airinv-paths.hpp>
#include <airinv/AIRINV_Master_Service.hpp>

// ///////// Type definitions //////////
typedef unsigned int ServerPort_T;

// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_AIRINV_DEFAULT_LOG_FILENAME ("airinvServer.log");

/** Default protocol to use for the server. */
const std::string K_AIRINV_DEFAULT_SERVER_PROTOCOL ("tcp://");

/** Default address to bind to. */
const std::string K_AIRINV_DEFAULT_SERVER_ADDRESS ("*");

/** Default port to bind to. */
const ServerPort_T K_AIRINV_DEFAULT_SERVER_PORT (5555);

/** Default name and location for the (CSV) input files. */
const std::string K_AIRINV_DEFAULT_INVENTORY_FILENAME (STDAIR_SAMPLE_DIR
                                                       "/invdump01.csv");
/** Default name and location for the (CSV) input files. */
const std::string K_AIRINV_DEFAULT_SCHEDULE_FILENAME (STDAIR_SAMPLE_DIR
                                                      "/schedule01.csv");
/** Default name and location for the (CSV) input files. */
const std::string K_AIRINV_DEFAULT_OND_FILENAME (STDAIR_SAMPLE_DIR
                                                 "/ond01.csv");

/** Default name and location for the (CSV) input files. */
const std::string K_AIRINV_DEFAULT_YIELD_FILENAME (STDAIR_SAMPLE_DIR
                                                   "/yield01.csv");

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
 * Enumeration representing the command entered by the user on the command-line.
 */
struct Command_T {
  typedef enum {
    NOP = 0,
    QUIT,
    DISPLAY,
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

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], std::string& ioServerProtocol,
                       std::string& ioServerAddress, ServerPort_T& ioServerPort,
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
     "(CVS) input file for the inventory")
    ("schedule,s",
     boost::program_options::value< std::string >(&ioScheduleInputFilename)->default_value(K_AIRINV_DEFAULT_SCHEDULE_FILENAME),
     "(CVS) input file for the schedule")
    ("ond,o",
     boost::program_options::value< std::string >(&ioODInputFilename)->default_value(K_AIRINV_DEFAULT_OND_FILENAME),
     "(CVS) input file for the O&D")
    ("yield,y",
     boost::program_options::value< std::string >(&ioYieldInputFilename)->default_value(K_AIRINV_DEFAULT_YIELD_FILENAME),
     "(CVS) input file for the yield")
    ("protocol,t",
     boost::program_options::value< std::string >(&ioServerProtocol)->default_value(K_AIRINV_DEFAULT_SERVER_PROTOCOL),
     "Server protocol")
    ("address,a",
     boost::program_options::value< std::string >(&ioServerAddress)->default_value(K_AIRINV_DEFAULT_SERVER_ADDRESS),
     "Server address")
    ("port,p",
     boost::program_options::value< ServerPort_T >(&ioServerPort)->default_value(K_AIRINV_DEFAULT_SERVER_PORT),
     "Server port")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_AIRINV_DEFAULT_LOG_FILENAME),
     "Filename for the output logs")
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

  std::ifstream ifs ("airinvServer.cfg");
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

  if (vm.count ("protocol")) {
    ioServerProtocol = vm["protocol"].as< std::string >();
    std::cout << "Server protocol is: " << ioServerProtocol << std::endl;
  }

  if (vm.count ("address")) {
    ioServerAddress = vm["address"].as< std::string >();
    std::cout << "Server address is: " << ioServerAddress << std::endl;
  }

  if (vm.count ("port")) {
    ioServerPort = vm["port"].as< ServerPort_T >();
    std::cout << "Server port is: " << ioServerPort << std::endl;
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


// ///////// Utility functions on top of the ZeroMQ library /////////
/**
 * Receive 0MQ string from socket and convert into string.
 */
static std::string s_recv (zmq::socket_t& socket) {
  zmq::message_t message;
  socket.recv (&message);

  return std::string (static_cast<char*> (message.data()), message.size());
}

/**
 * Convert string to 0MQ string and send to socket.
 */
static bool s_send (zmq::socket_t& socket, const std::string& string) {
  zmq::message_t message (string.size());
  memcpy (message.data(), string.data(), string.size());

  bool rc = socket.send (message);
  return rc;
}


// /////////////////////// M A I N ////////////////////////
int main (int argc, char* argv[]) {

  // Server parameters (for ZeroMQ)
  std::string ioServerProtocol;
  std::string ioServerAddress;
  ServerPort_T ioServerPort;

  // State whether the BOM tree should be built-in or parsed from an
  // input file
  bool isBuiltin;
  bool isForSchedule;

  // Input file names
  stdair::Filename_T lInventoryFilename;
  stdair::Filename_T lScheduleInputFilename;
  stdair::Filename_T lODInputFilename;
  stdair::Filename_T lYieldInputFilename;

  // Output log File
  stdair::Filename_T lLogFilename;

  // Call the command-line option parser
  const int lOptionParserStatus =
    readConfiguration (argc, argv, ioServerProtocol, ioServerAddress,
                       ioServerPort, isBuiltin, isForSchedule,
                       lInventoryFilename, lScheduleInputFilename,
                       lODInputFilename, lYieldInputFilename, lLogFilename);

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
  STDAIR_LOG_DEBUG ("Initialisation of the AirInv server");

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {

    // Build the sample BOM tree for RMOL
    airinvService.buildSampleBom();

  } else {
    if (isForSchedule == true) {
      // Build the BOM tree from parsing a schedule file (and O&D list)
      airinvService.parseAndLoad (lScheduleInputFilename, lODInputFilename,
                                  lYieldInputFilename);

    } else {
      // Build the BOM tree from parsing an inventory dump file
      airinvService.parseAndLoad (lInventoryFilename);
    }
  }

  // Build the connection string (e.g., "tcp://*:5555", which is the default)
  std::ostringstream oZeroMQBindStream;
  oZeroMQBindStream << ioServerProtocol << ioServerAddress
                    << ":" << ioServerPort;
  const std::string lZeroMQBindString (oZeroMQBindStream.str());

  // Prepare the context and socket of the server
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  socket.bind (lZeroMQBindString.c_str());

  // DEBUG
  STDAIR_LOG_DEBUG ("The AirInv server is ready to receive requests...");

  while (true) {

    // Wait for next request from client, which is expected to give
    // the JSON-ified details of the requested flight-date
    const std::string& lFlightDateKeyJSONString = s_recv (socket);

    // DEBUG
    STDAIR_LOG_DEBUG ("Received: '" << lFlightDateKeyJSONString << "'");

    // Extract, from the JSON-ified string an airline code
    stdair::AirlineCode_T lAirlineCode;
    stdair::BomJSONImport::jsonImportInventoryKey (lFlightDateKeyJSONString,
                                                   lAirlineCode);

    // Extract, from the JSON-ified string a flight number and a departure date
    stdair::FlightNumber_T lFlightNumber;
    stdair::Date_T lDate;
    stdair::BomJSONImport::jsonImportFlightDateKey (lFlightDateKeyJSONString,
                                                    lFlightNumber, lDate);

    // DEBUG
    STDAIR_LOG_DEBUG ("=> airline code = '" << lAirlineCode
                      << "', flight number = " << lFlightNumber
                      << "', departure date = '" << lDate << "'");

    // DEBUG: Display the flight-date dump
    const std::string& lFlightDateCSVDump =
      airinvService.csvDisplay (lAirlineCode, lFlightNumber, lDate);
    STDAIR_LOG_DEBUG (std::endl << lFlightDateCSVDump);

    // Dump the full details of the flight-date into the JSON-ified flight-date
    const std::string& lFlightDateJSONDump =
      airinvService.jsonExport (lAirlineCode, lFlightNumber, lDate);

    // DEBUG
    STDAIR_LOG_DEBUG ("Send: '" << lFlightDateJSONDump << "'");

    // Send back the flight-date details to the client
    s_send (socket, lFlightDateJSONDump);
  }

  return 0;
}

