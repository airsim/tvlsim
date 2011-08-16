// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
//  //// Boost (Extended STL) ////
// Boost Tokeniser
#include <boost/tokenizer.hpp>
// Boost Program Options
#include <boost/program_options.hpp>
// Boost Accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
// Boost Progress
//#include <boost/progress.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BomDisplay.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/config/trademgen-paths.hpp>
#include <trademgen/config/trademgen-paths.hpp>

// Aliases for namespaces
namespace ba = boost::accumulators;

// //////// Specific type definitions ///////
typedef unsigned int NbOfRuns_T;

/**
 * Type definition to gather statistics.
 */
typedef ba::accumulator_set<double,
                            ba::stats<ba::tag::min, ba::tag::max,
                                      ba::tag::mean (ba::immediate),
                                      ba::tag::sum,
                                      ba::tag::variance> > stat_acc_type;

// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_LOG_FILENAME ("trademgen.log");

/**
 * Default name and location for the (CSV) input file.
 */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                             "/demand01.csv");

/**
 * Default name and location for the (CSV) output file.
 */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_OUTPUT_FILENAME ("request.csv");

/**
 * Default demand generation method: Poisson Process.
 */
const stdair::DemandGenerationMethod
K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD =
  stdair::DemandGenerationMethod::POI_PRO;

/**
 * Default demand generation method name: 'P' for Poisson Process.
 */
const char K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD_CHAR =
  K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD.getMethodAsChar();

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
 * Default for the input type. It can be either built-in or provided by an
 * input file. That latter must then be given with the -i option.
 */
const bool K_TRADEMGEN_DEFAULT_BUILT_IN_INPUT = false;

/**
 * Early return status (so that it can be differentiated from an error).
 */
const int K_TRADEMGEN_EARLY_RETURN_STATUS = 99;


/**
 * Display the statistics held by the dedicated accumulator.
 */
void stat_display (std::ostream& oStream, const stat_acc_type& iStatAcc) {

  // Store current formatting flags of the output stream
  std::ios::fmtflags oldFlags = oStream.flags();

  //
  oStream.setf (std::ios::fixed);
  
  //
  oStream << "Statistics for the demand generation runs: " << std::endl;
  oStream << "  minimum   = " << ba::min (iStatAcc) << std::endl;
  oStream << "  mean      = " << ba::mean (iStatAcc) << std::endl;
  oStream << "  maximum   = " << ba::max (iStatAcc) << std::endl;
  oStream << "  count     = " << ba::count (iStatAcc) << std::endl;
  oStream << "  variance  = " << ba::variance (iStatAcc) << std::endl;
  
  // Reset formatting flags of output stream
  oStream.flags (oldFlags);
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
int readConfiguration (int argc, char* argv[], bool& ioIsBuiltin,
                       stdair::RandomSeed_T& ioRandomSeed,
                       NbOfRuns_T& ioRandomRuns,
                       stdair::Filename_T& ioInputFilename,
                       stdair::Filename_T& ioOutputFilename,
                       stdair::Filename_T& ioLogFilename,
                       stdair::DemandGenerationMethod& ioDemandGenerationMethod) {

  // Demand generation method as a single char (e.g., 'P' or 'S').
  char lDemandGenerationMethodChar;

  // Default for the built-in input
  ioIsBuiltin = K_TRADEMGEN_DEFAULT_BUILT_IN_INPUT;

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
    ("seed,s",
     boost::program_options::value<stdair::RandomSeed_T>(&ioRandomSeed)->default_value(K_TRADEMGEN_DEFAULT_RANDOM_SEED),
     "Seed for the random generation")
    ("draws,d",
     boost::program_options::value<NbOfRuns_T>(&ioRandomRuns)->default_value(K_TRADEMGEN_DEFAULT_RANDOM_DRAWS), 
     "Number of runs for the demand generations")
    ("input,i",
     boost::program_options::value< std::string >(&ioInputFilename)->default_value(K_TRADEMGEN_DEFAULT_INPUT_FILENAME),
     "(CVS) input file for the demand distributions")
    ("output,o",
     boost::program_options::value< std::string >(&ioOutputFilename)->default_value(K_TRADEMGEN_DEFAULT_OUTPUT_FILENAME),
     "(CVS) output file for the generated requests")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_TRADEMGEN_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ("demandgeneration,G",
     boost::program_options::value< char >(&lDemandGenerationMethodChar)->default_value(K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD_CHAR),
     "Method used to generate the demand (i.e., the booking requests): Poisson Process (P) or Order Statistics (S)")
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

  std::ifstream ifs ("trademgen.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_TRADEMGEN_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_TRADEMGEN_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_TRADEMGEN_EARLY_RETURN_STATUS;
  }

  if (vm.count ("builtin")) {
    ioIsBuiltin = true;
  }
  const std::string isBuiltinStr = (ioIsBuiltin == true)?"yes":"no";
  std::cout << "The BOM should be built-in? " << isBuiltinStr << std::endl;

  if (ioIsBuiltin == false) {

    // The BOM tree should be built from parsing a demand input file
    if (vm.count ("input")) {
      ioInputFilename = vm["input"].as< std::string >();
      std::cout << "Input filename is: " << ioInputFilename << std::endl;

    } else {
      // The built-in option is not selected. However, no demand input file
      // is specified
      std::cerr << "Either one among the -b/--builtin and -i/--input "
                << "options must be specified" << std::endl;
    }
  }

  if (vm.count ("output")) {
    ioOutputFilename = vm["output"].as< std::string >();
    std::cout << "Output filename is: " << ioOutputFilename << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  if (vm.count ("demandgeneration")) {
    ioDemandGenerationMethod =
      stdair::DemandGenerationMethod (lDemandGenerationMethodChar);
    std::cout << "Date-time request generation method is: "
              << ioDemandGenerationMethod.describe() << std::endl;
  }

  //
  std::cout << "The random generation seed is: " << ioRandomSeed << std::endl;

  //
  std::cout << "The number of runs is: " << ioRandomRuns << std::endl;
  
  return 0;
}

// /////////////////////////////////////////////////////////////////////////
void generateDemand (TRADEMGEN::TRADEMGEN_Service& ioTrademgenService,
                     const stdair::Filename_T& iOutputFilename,
                     const NbOfRuns_T& iNbOfRuns,
                     const stdair::DemandGenerationMethod& iDemandGenerationMethod) {

  // Open and clean the .csv output file
  std::ofstream output;
  output.open (iOutputFilename.c_str());
  output.clear();
    
  // Initialise the statistics collector/accumulator
  stat_acc_type lStatAccumulator;

  // Retrieve the expected (mean value of the) number of events to be
  // generated
  const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
    ioTrademgenService.getExpectedTotalNumberOfRequestsToBeGenerated();

  // Initialise the (Boost) progress display object
  boost::progress_display lProgressDisplay (lExpectedNbOfEventsToBeGenerated
                                            * iNbOfRuns);
  
  for (NbOfRuns_T runIdx = 1; runIdx <= iNbOfRuns; ++runIdx) {
    // /////////////////////////////////////////////////////
    output << "Run number: " << runIdx << std::endl;

    /**
       Initialisation step.
       <br>Generate the first event for each demand stream.
    */
    const stdair::Count_T& lActualNbOfEventsToBeGenerated =
      ioTrademgenService.generateFirstRequests (iDemandGenerationMethod);

    // DEBUG
    STDAIR_LOG_DEBUG ("[" << runIdx << "] Expected: "
                      << lExpectedNbOfEventsToBeGenerated << ", actual: "
                      << lActualNbOfEventsToBeGenerated);
      
    /**
       Main loop.
       <ul>
       <li>Pop a request and get its associated type/demand stream.</li>
       <li>Generate the next request for the same type/demand stream.</li>
       </ul>
    */
    while (ioTrademgenService.isQueueDone() == false) {

      // Extract the next event from the event queue
      stdair::EventStruct lEventStruct;
      stdair::ProgressStatusSet lProgressStatusSet =
        ioTrademgenService.popEvent (lEventStruct);
      
      // DEBUG
      // STDAIR_LOG_DEBUG ("[" << runIdx << "] Poped event: '"
      //                   << lEventStruct.describe() << "'.");
      
      // Extract the corresponding demand/booking request
      const stdair::BookingRequestStruct& lPoppedRequest =
        lEventStruct.getBookingRequest();
        
      // DEBUG
      STDAIR_LOG_DEBUG ("[" << runIdx << "] Poped booking request: '"
                        << lPoppedRequest.describe() << "'.");
    
      // Dump the request into the dedicated CSV file
      // stdair::BomDisplay::csvDisplay (output, lPoppedRequest);
        
      // Retrieve the corresponding demand stream key
      const stdair::DemandGeneratorKey_T& lDemandStreamKey =
        lPoppedRequest.getDemandGeneratorKey();
      
      // Assess whether more events should be generated for that demand stream
      const bool stillHavingRequestsToBeGenerated = ioTrademgenService.
        stillHavingRequestsToBeGenerated (lDemandStreamKey,
                                          lProgressStatusSet,
                                          iDemandGenerationMethod);

      // DEBUG
      STDAIR_LOG_DEBUG (lProgressStatusSet.describe());
      STDAIR_LOG_DEBUG ("=> [" << lDemandStreamKey << "] is now processed. "
                        << "Still generate events for that demand stream? "
                        << stillHavingRequestsToBeGenerated);
    
      // If there are still events to be generated for that demand stream,
      // generate and add them to the event queue
      if (stillHavingRequestsToBeGenerated == true) {
        
        stdair::BookingRequestPtr_T lNextRequest_ptr =
          ioTrademgenService.generateNextRequest (lDemandStreamKey,
                                                  iDemandGenerationMethod);
        
        assert (lNextRequest_ptr != NULL);

        // Sanity check
        const stdair::Duration_T lDuration =
          lNextRequest_ptr->getRequestDateTime()
          - lPoppedRequest.getRequestDateTime();
        if (lDuration.total_milliseconds() < 0) {
          STDAIR_LOG_ERROR ("[" << lDemandStreamKey
                            << "] The date-time of the generated event ("
                            << lNextRequest_ptr->getRequestDateTime()
                            << ") is lower than the date-time "
                            << "of the current event ("
                            << lPoppedRequest.getRequestDateTime() << ")");
          assert (false);
        }

        // DEBUG
        STDAIR_LOG_DEBUG ("[" << lDemandStreamKey << "] Added request: '"
                          << lNextRequest_ptr->describe()
                          << "'. Is queue done? "
                          << ioTrademgenService.isQueueDone());
      }
      // DEBUG
      STDAIR_LOG_DEBUG ("");
      
      // Update the progress display
      ++lProgressDisplay;
    }

    // Add the number of events to the statistics accumulator
    lStatAccumulator (lActualNbOfEventsToBeGenerated);
    
    // Reset the service (including the event queue) for the next run
    ioTrademgenService.reset();
  }

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the demand generation. Following are some "
                    "statistics for the " << iNbOfRuns << " runs.");
  std::ostringstream oStatStr;
  stat_display (oStatStr, lStatAccumulator);
  STDAIR_LOG_DEBUG (oStatStr.str());

  // DEBUG
  const std::string& lBOMStr = ioTrademgenService.csvDisplay();
  STDAIR_LOG_DEBUG (lBOMStr);

  // Close the output file
  output.close();
}


// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  // State whether the BOM tree should be built-in or parsed from an input file
  bool isBuiltin;

  // Random generation seed
  stdair::RandomSeed_T lRandomSeed;

  // Number of random draws to be generated (best if greater than 100)
  NbOfRuns_T lNbOfRuns;
    
  // Input file name
  stdair::Filename_T lInputFilename;

  // Output file name
  stdair::Filename_T lOutputFilename;

  // Output log File
  stdair::Filename_T lLogFilename;
  
  // Demand generation method.
  stdair::DemandGenerationMethod
    lDemandGenerationMethod (K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD);

  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, isBuiltin, lRandomSeed, lNbOfRuns,
                       lInputFilename, lOutputFilename, lLogFilename,
                       lDemandGenerationMethod);

  if (lOptionParserStatus == K_TRADEMGEN_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Set up the log parameters
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);

  // Initialise the TraDemGen service object
  TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lRandomSeed);

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {
    // Create a sample DemandStream object, and insert it within the BOM tree
    trademgenService.buildSampleBom();

  } else {
    // Create the DemandStream objects, and insert them within the BOM tree
    trademgenService.parseAndLoad (lInputFilename);
  }  

  // Calculate the expected number of events to be generated.
  generateDemand (trademgenService, lOutputFilename, lNbOfRuns,
                  lDemandGenerationMethod);

  // Close the Log outputFile
  logOutputFile.close();

  /*
    \note: as that program is not intended to be run on a server in
    production, it is better not to catch the exceptions. When it
    happens (that an exception is throwned), that way we get the
    call stack.
  */

  return 0;
}
