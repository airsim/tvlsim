// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
// Boost (Extended STL)
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/basic/PartnershipTechnique.hpp>
#include <stdair/service/Logger.hpp>
// DSIM
#include <dsim/DSIM_Service.hpp>
#include <dsim/config/dsim-paths.hpp>

// //////// Type definitions ///////
/**
 * Number of runs to be performed by the simulation.
 */
typedef unsigned int NbOfRuns_T;

/**
 * List of workds for a query.
 */
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_DSIM_DEFAULT_LOG_FILENAME ("simulate.log");

/**
 * Default name and location for the (CSV) schedule input file.
 */
const std::string K_DSIM_DEFAULT_SCHEDULE_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                          "/rds01/schedule.csv");

/**
 * Default name and location for the (CSV) O&D input file.
 */
const std::string K_DSIM_DEFAULT_OND_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                     "/ond01.csv");

/**
 * Default name and location for the (CSV) yield input file.
 */
const std::string K_DSIM_DEFAULT_YIELD_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                      "/rds01/yield.csv");
    
/**
 * Default name and location for the (CSV) fare input file.
 */
const std::string K_DSIM_DEFAULT_FARE_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                      "/rds01/fare.csv");

/**
 * Default name and location for the (CSV) demand input file.
 */
const std::string K_DSIM_DEFAULT_DEMAND_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                        "/rds01/demand.csv");

/**
 * Default forecasting method name: 'M' for MultiplicativePickUp.
 */
const char K_DSIM_DEFAULT_FORECASTING_METHOD_CHAR ('M');

/**
 * Default demand generation method name: 'S' for Statistics Order.
 */
const char K_DSIM_DEMAND_GENERATION_METHOD_CHAR ('S');

/**
 * Default partnership technique name: 'N' for None ().
 */
const char K_DSIM_PARTNERSHIP_TECHNIQUE_CHAR ('N');

/**
 * Default random generation seed (e.g., 120765987).
 */
const stdair::RandomSeed_T K_TRADEMGEN_DEFAULT_RANDOM_SEED =
  stdair::DEFAULT_RANDOM_SEED;

/**
 * Default number of random draws to be generated (best if over 100).
 */
const NbOfRuns_T K_TRADEMGEN_DEFAULT_RANDOM_DRAWS = 1;

/**
 * Default for the BOM tree building. The BOM tree can either be built-in
 * or provided by an input file. That latter must then be given with input
 * file options (-d, -s, -o, -f, -y).
 */
const bool K_DSIM_DEFAULT_BUILT_IN_INPUT = false;

/**
 * Default query string.
 */
const std::string K_DSIM_DEFAULT_QUERY_STRING ("my good old query");

/**
 * Default name and location for the Xapian database.
 */
const std::string K_DSIM_DEFAULT_DB_USER ("dsim");
const std::string K_DSIM_DEFAULT_DB_PASSWD ("dsim");
const std::string K_DSIM_DEFAULT_DB_DBNAME ("sim_dsim");
const std::string K_DSIM_DEFAULT_DB_HOST ("localhost");
const std::string K_DSIM_DEFAULT_DB_PORT ("3306");


// //////////////////////////////////////////////////////////////////////
void tokeniseStringIntoWordList (const std::string& iPhrase,
                                 WordList_T& ioWordList) {
  // Empty the word list
  ioWordList.clear();
  
  // Boost Tokeniser
  typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;
  
  // Define the separators
  const boost::char_separator<char> lSepatorList(" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");
  
  // Initialise the phrase to be tokenised
  Tokeniser_T lTokens (iPhrase, lSepatorList);
  for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
       tok_iter != lTokens.end(); ++tok_iter) {
    const std::string& lTerm = *tok_iter;
    ioWordList.push_back (lTerm);
  }
  
}

// //////////////////////////////////////////////////////////////////////
std::string createStringFromWordList (const WordList_T& iWordList) {
  std::ostringstream oStr;

  unsigned short idx = iWordList.size();
  for (WordList_T::const_iterator itWord = iWordList.begin();
       itWord != iWordList.end(); ++itWord, --idx) {
    const std::string& lWord = *itWord;
    oStr << lWord;
    if (idx > 1) {
      oStr << " ";
    }
  }
  
  return oStr.str();
}


// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/** Early return status (so that it can be differentiated from an error). */
const int K_DSIM_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], 
                       bool& ioIsBuiltin, stdair::RandomSeed_T& ioRandomSeed,
                       NbOfRuns_T& ioRandomRuns, std::string& ioQueryString,
                       stdair::Filename_T& ioScheduleInputFilename,
                       stdair::Filename_T& ioOnDInputFilename,
                       stdair::Filename_T& ioYieldInputFilename,
                       stdair::Filename_T& ioFareInputFilename,
                       stdair::Filename_T& ioDemandInputFilename,
                       std::string& ioLogFilename,
                       stdair::ForecastingMethod& ioForecastingMethod,
                       stdair::DemandGenerationMethod& ioDemandGenerationMethod,
                       stdair::PartnershipTechnique& ioPartnershipTechnique,
                       std::string& ioDBUser, std::string& ioDBPasswd,
                       std::string& ioDBHost, std::string& ioDBPort,
                       std::string& ioDBDBName) {

  // Forecast method as a single char (e.g., 'A' or 'M').
  char lForecastingMethodChar;
  // Demand generation method as a single char (e.g., 'P' or 'S').
  char lDemandGenerationMethodChar;
  // Partnership technique as a single char (e.g., 'r' or 'C').
  char lPartnershipTechniqueChar;

  // Default for the built-in input
  ioIsBuiltin = K_DSIM_DEFAULT_BUILT_IN_INPUT;

  // Initialise the travel query string, if that one is empty
  if (ioQueryString.empty() == true) {
    ioQueryString = K_DSIM_DEFAULT_QUERY_STRING;
  }
  
  // Transform the query string into a list of words (STL strings)
  WordList_T lWordList;
  tokeniseStringIntoWordList (ioQueryString, lWordList);

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
     "The sample BOM tree can be either built-in or parsed from input files. In that latter case, the input files must be specified as well (e.g., -d/--demand, -s/--schedule,  -o/--ond, -f/--fare, -y/--yield)")
    ("seed,S",
     boost::program_options::value<stdair::RandomSeed_T>(&ioRandomSeed)->default_value(K_TRADEMGEN_DEFAULT_RANDOM_SEED),
     "Seed for the random generation")
    ("runs,r",
     boost::program_options::value<NbOfRuns_T>(&ioRandomRuns)->default_value(K_TRADEMGEN_DEFAULT_RANDOM_DRAWS),
     "Number of simulation runs")
    ("schedule,s",
     boost::program_options::value< std::string >(&ioScheduleInputFilename)->default_value(K_DSIM_DEFAULT_SCHEDULE_INPUT_FILENAME),
     "(CSV) input file for the schedules")
    ("ond,o",
     boost::program_options::value< std::string >(&ioOnDInputFilename)->default_value(K_DSIM_DEFAULT_OND_INPUT_FILENAME),
     "(CSV) input file for the O&D definitions")
    ("yield,y",
     boost::program_options::value< std::string >(&ioYieldInputFilename)->default_value(K_DSIM_DEFAULT_YIELD_INPUT_FILENAME),
     "(CSV) input file for the yields")
    ("fare,f",
     boost::program_options::value< std::string >(&ioFareInputFilename)->default_value(K_DSIM_DEFAULT_FARE_INPUT_FILENAME),
     "(CSV) input file for the fares")
    ("demand,d",
     boost::program_options::value< std::string >(&ioDemandInputFilename)->default_value(K_DSIM_DEFAULT_DEMAND_INPUT_FILENAME),
     "(CSV) input file for the demand distributions")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_DSIM_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ("forecast,F",
     boost::program_options::value< char >(&lForecastingMethodChar)->default_value(K_DSIM_DEFAULT_FORECASTING_METHOD_CHAR),
     "Method used to forecast demand: Additive Pick-Up (e.g., A) or Multiplicative Pick-Up (e.g., M)")
    ("demandgeneration,G",
     boost::program_options::value< char >(&lDemandGenerationMethodChar)->default_value(K_DSIM_DEMAND_GENERATION_METHOD_CHAR),
     "Method used to generate the demand (i.e., booking requests): Poisson Process (e.g., P) or Statistics Order (e.g., S)")
    ("partnership,T",
     boost::program_options::value< char >(&lPartnershipTechniqueChar)->default_value(K_DSIM_PARTNERSHIP_TECHNIQUE_CHAR),
     "Technique used in a partnership (defines both revenue management and inventory control methods): None (N), Revenue Availability Exchange using Demand Aggregation (r), Revenue Availability Exchange using Yield Proration (R), Interline Bid Price using Demand Aggregation (i), Interline Bid Price using Yield Proration (I), Interline Bid Price using Yield Proration without protection (U), Revenue Management Cooperation (C) or Advanced Revenue Management Cooperation (A)")
    ("user,u",
     boost::program_options::value< std::string >(&ioDBUser)->default_value(K_DSIM_DEFAULT_DB_USER),
     "SQL database hostname (e.g., dsim)")
    ("passwd,p",
     boost::program_options::value< std::string >(&ioDBPasswd)->default_value(K_DSIM_DEFAULT_DB_PASSWD),
     "SQL database hostname (e.g., dsim)")
    ("host,H",
     boost::program_options::value< std::string >(&ioDBHost)->default_value(K_DSIM_DEFAULT_DB_HOST),
     "SQL database hostname (e.g., localhost)")
    ("port,P",
     boost::program_options::value< std::string >(&ioDBPort)->default_value(K_DSIM_DEFAULT_DB_PORT),
     "SQL database port (e.g., 3306)")
    ("dbname,n",
     boost::program_options::value< std::string >(&ioDBDBName)->default_value(K_DSIM_DEFAULT_DB_DBNAME),
     "SQL database name (e.g., dsim)")
    ("query,q",
     boost::program_options::value< WordList_T >(&lWordList)->multitoken(),
     "Query word list")
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

  std::ifstream ifs ("simulate.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_DSIM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_DSIM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_DSIM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("builtin")) {
    ioIsBuiltin = true;
  }
  const std::string isBuiltinStr = (ioIsBuiltin == true)?"yes":"no";
  std::cout << "The BOM should be built-in? " << isBuiltinStr << std::endl;

  //
  std::ostringstream oErrorMessageStr;
  oErrorMessageStr << "Either the -b/--builtin option, or the combination of "
                   << "the -d/--demand, -s/--schedule, -o/--ond, -f/--fare "
                   << "and -y/--yield options must be specified";

  if (ioIsBuiltin == false) {
    if (vm.count ("schedule")) {
      ioScheduleInputFilename = vm["schedule"].as< std::string >();
      std::cout << "Schedule input filename is: " << ioScheduleInputFilename
                << std::endl;

    } else {
      // The built-in option is not selected. However, no schedule input file
      // is specified
      std::cerr << oErrorMessageStr.str() << std::endl;
    }

    if (vm.count ("ond")) {
      ioOnDInputFilename = vm["ond"].as< std::string >();
      std::cout << "O&D input filename is: " << ioOnDInputFilename << std::endl;

    } else {
      // The built-in option is not selected. However, no schedule input file
      // is specified
      std::cerr << oErrorMessageStr.str() << std::endl;
    }

    if (vm.count ("yield")) {
      ioYieldInputFilename = vm["yield"].as< std::string >();
      std::cout << "Yield input filename is: " << ioYieldInputFilename << std::endl;

    } else {
      // The built-in option is not selected. However, no schedule input file
      // is specified
      std::cerr << oErrorMessageStr.str() << std::endl;
    }

    if (vm.count ("fare")) {
      ioFareInputFilename = vm["fare"].as< std::string >();
      std::cout << "Fare input filename is: " << ioFareInputFilename << std::endl;

    } else {
      // The built-in option is not selected. However, no schedule input file
      // is specified
      std::cerr << oErrorMessageStr.str() << std::endl;
    }

    if (vm.count ("demand")) {
      ioDemandInputFilename = vm["demand"].as< std::string >();
      std::cout << "Demand input filename is: " << ioDemandInputFilename
                << std::endl;
    } else {
      // The built-in option is not selected. However, no schedule input file
      // is specified
      std::cerr << oErrorMessageStr.str() << std::endl;
    }
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  //
  if (vm.count ("forecast")) {
    ioForecastingMethod = stdair::ForecastingMethod (lForecastingMethodChar);
    std::cout << "Forecasting method is: " << ioForecastingMethod.describe() << std::endl;
  }

  //
  if (vm.count ("demandgeneration")) {
    ioDemandGenerationMethod =
      stdair::DemandGenerationMethod (lDemandGenerationMethodChar);
    std::cout << "Demand generation method is: "
              << ioDemandGenerationMethod.describe() << std::endl;
  }

  if (vm.count ("partnership")) {
    ioPartnershipTechnique =
      stdair::PartnershipTechnique (lPartnershipTechniqueChar);
    std::cout << "Partnership technique is: "
              << ioPartnershipTechnique.describe() << std::endl;
  }

  //
  std::cout << "The random generation seed is: " << ioRandomSeed << std::endl;

  //
  std::cout << "The number of simulation runs is: " << ioRandomRuns
            << std::endl;

  //
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

  //
  ioQueryString = createStringFromWordList (lWordList);
  std::cout << "The query string is: " << ioQueryString << std::endl;
  
  return 0;
}

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // State whether the BOM tree should be built-in or parsed from an
  // input file
  bool isBuiltin;

  // Random generation seed
  stdair::RandomSeed_T lRandomSeed;

  // Number of simulation runs to be performed
  NbOfRuns_T lNbOfRuns;

  // Query
  std::string lQuery;

  // Start date
  stdair::Date_T lStartDate (2009, boost::gregorian::Feb, 01);
  
  // End date
  stdair::Date_T lEndDate (2012, boost::gregorian::Mar, 02);

  // Schedule input file name
  stdair::Filename_T lScheduleInputFilename;

  // O&D input filename
  std::string lOnDInputFilename;
    
  // Yield input filename
  std::string lYieldInputFilename;
    
  // Fare input filename
  std::string lFareInputFilename;
    
  // Demand input file name
  stdair::Filename_T lDemandInputFilename;

  // Output log File
  std::string lLogFilename;

  // Forecasting method.
  stdair::ForecastingMethod
    lForecastingMethod (K_DSIM_DEFAULT_FORECASTING_METHOD_CHAR);

  // Demand generation method.
  stdair::DemandGenerationMethod
    lDemandGenerationMethod (K_DSIM_DEMAND_GENERATION_METHOD_CHAR);

  // Partnership technique.
  stdair::PartnershipTechnique lPartnershipTechnique(K_DSIM_PARTNERSHIP_TECHNIQUE_CHAR);

  // SQL database parameters
  std::string lDBUser;
  std::string lDBPasswd;
  std::string lDBHost;
  std::string lDBPort;
  std::string lDBDBName;
                       
  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, isBuiltin, lRandomSeed, lNbOfRuns, lQuery,
                       lScheduleInputFilename, lOnDInputFilename,
                       lYieldInputFilename, lFareInputFilename,
                       lDemandInputFilename, lLogFilename,
                       lForecastingMethod, lDemandGenerationMethod, lPartnershipTechnique, 
                       lDBUser, lDBPasswd, lDBHost, lDBPort, lDBDBName);

  if (lOptionParserStatus == K_DSIM_EARLY_RETURN_STATUS) {
    return 0;
  }
    
  // Set the database parameters
  stdair::BasDBParams lDBParams (lDBUser, lDBPasswd, lDBHost, lDBPort,
                                 lDBDBName);
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the simulation context
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  DSIM::DSIM_Service dsimService (lLogParams, lDBParams, lStartDate, lEndDate,
                                  lRandomSeed);

  // Check wether or not (CSV) input files should be read
  if (isBuiltin == true) {

    // Build the sample BOM tree
    dsimService.buildSampleBom();

  } else {
    // Build the BOM tree from parsing input files
    dsimService.parseAndLoad (lScheduleInputFilename, lOnDInputFilename,
                              lYieldInputFilename, lFareInputFilename,
                              lDemandInputFilename);
  }

  // Initialise the snapshot and RM events
  dsimService.initSnapshotAndRMEvents();

  // Perform a simulation
  dsimService.simulate (lNbOfRuns, lDemandGenerationMethod, lForecastingMethod, lPartnershipTechnique);

  
  // DEBUG
  // Display the airlines stored in the database
  dsimService.displayAirlineListFromDB();

  return 0;	
}

