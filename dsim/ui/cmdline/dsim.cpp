/*!
 * \page batch_airinv_cpp Command-Line Utility to Demonstrate Typical AirInv Usage
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
#include <boost/swap.hpp>
#include <boost/algorithm/string/case_conv.hpp>
// GNU Readline Wrapper
#include <stdair/ui/cmdline/SReadline.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_json.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/basic/PartnershipTechnique.hpp>
#include <stdair/service/Logger.hpp>
// DSIM
#include <dsim/DSIM_Service.hpp>
#include <dsim/DSIM_Types.hpp>
#include <dsim/config/dsim-paths.hpp>

// //////// Type definitions ///////
/**
 * List of workds for a query.
 */
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_DSIM_DEFAULT_LOG_FILENAME ("dsim.log");

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
const DSIM::NbOfRuns_T K_TRADEMGEN_DEFAULT_RANDOM_DRAWS = 1;

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
 * Early return status (so that it can be differentiated from an error).
 */
const int K_DSIM_EARLY_RETURN_STATUS = 99;

/**
 * Default name and location for the Xapian database.
 */
const std::string K_DSIM_DEFAULT_DB_USER ("dsim");
const std::string K_DSIM_DEFAULT_DB_PASSWD ("dsim");
const std::string K_DSIM_DEFAULT_DB_DBNAME ("sim_dsim");
const std::string K_DSIM_DEFAULT_DB_HOST ("localhost");
const std::string K_DSIM_DEFAULT_DB_PORT ("3306");


// //////////////////////////////////////////////////////////////////////
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
    JSON_EVENT_LIST,
    LAST_VALUE
  } Type_T;
};

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
  
}// //////////////////////////////////////////////////////////////////////
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

/**
 * Read and parse the command line options.
 */
int readConfiguration (int argc, char* argv[], 
                       bool& ioIsBuiltin, stdair::RandomSeed_T& ioRandomSeed,
                       DSIM::NbOfRuns_T& ioRandomRuns, std::string& ioQueryString,
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
     boost::program_options::value<DSIM::NbOfRuns_T>(&ioRandomRuns)->default_value(K_TRADEMGEN_DEFAULT_RANDOM_DRAWS),
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

// //////////////////////////////////////////////////////////////////
void initReadline (swift::SReadline& ioInputReader) {

  // Prepare the list of my own completers
  std::vector<std::string> Completers;

  // The following is supported:
  // - "identifiers"
  // - special identifier %file - means to perform a file name completion
  Completers.push_back ("help");
  Completers.push_back ("json_event_list");
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
    std::string lCommand (*itTok);
    boost::algorithm::to_lower (lCommand);
    
    if (lCommand == "help") {
      oCommandType = Command_T::HELP;

    } else if (lCommand == "json_event_list") {
      oCommandType = Command_T::JSON_EVENT_LIST;

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

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // Readline history
  const unsigned int lHistorySize (100);
  const std::string lHistoryFilename ("dsim.hist");
  const std::string lHistoryBackupFilename ("dsim.hist.bak");
  
  // State whether the BOM tree should be built-in or parsed from an
  // input file
  bool isBuiltin;

  // Random generation seed
  stdair::RandomSeed_T lRandomSeed;

  // Number of simulation runs to be performed
  DSIM::NbOfRuns_T lNbOfRuns;

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
    const SIMFQT::FareFilePath lFareFilePath (lFareInputFilename);
    const AIRRAC::YieldFilePath lYieldFilePath (lYieldInputFilename); 
    dsimService.parseAndLoad (lScheduleInputFilename, lOnDInputFilename,
                              lYieldFilePath, lFareFilePath,
                              lDemandInputFilename);
  }

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
    // Prompt
    std::ostringstream oPromptStr;
    oPromptStr << "dsim "
               << "> ";
    // Call read-line, which will fill the list of tokens
    TokenList_T lTokenListByReadline;
    lUserInput = lReader.GetLine (oPromptStr.str(), lTokenListByReadline,
                                  EndOfInput);

    // The history can be saved to an arbitrary file at any time
    lReader.SaveHistory (lHistoryBackupFilename);

    // The end-of-input typically corresponds to a CTRL-D typed by the user
    if (EndOfInput) {
      std::cout << std::endl;
      break;
    }

    // Interpret the user input
    lCommandType = extractCommand (lTokenListByReadline);

    switch (lCommandType) {

      // ////////////////////////////// Help ////////////////////////
    case Command_T::HELP: {
      std::cout << std::endl;
      std::cout << "Commands: " << std::endl;
      std::cout << " help" << "\t\t" << "Display this help" << std::endl;
      std::cout << " quit" << "\t\t" << "Quit the application" << std::endl;
      std::cout << " \nDebug Commands" << std::endl;
      std::cout << " json_list" << "\t"
                << "List airlines, flights and departure dates in a JSON format"
                << std::endl;
      std::cout << std::endl;
      break;
    }
 
      // ////////////////////////////// Quit ////////////////////////
    case Command_T::QUIT: {
      break;
    }

      // ////////////////////////////// JSon Event List ////////////////////////

    case Command_T::JSON_EVENT_LIST: {
      //
      std::cout << "JSON Event List" << std::endl;

      std::ostringstream lMyCommandJSONstream;
      lMyCommandJSONstream << "{\"event_list\":"
                           << "{ \"event_type\":\"" << "all"
			   << "\"}}";

      // Delegate the call to the dedicated service
      const stdair::JSONString lJSONCommandString (lMyCommandJSONstream.str());
      const std::string& lCSVEventListDump =
        dsimService.jsonHandler (lJSONCommandString);

      // DEBUG: Display the events queue JSON string
      std::cout << lCSVEventListDump << std::endl;
      STDAIR_LOG_DEBUG (lCSVEventListDump);
      
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
      oStr << "That command is not yet understood: '" << lUserInput
           << "' => " << lTokenListByReadline;
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
