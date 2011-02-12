// STL
#include <cassert>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
//  //// Boost (Extended STL) ////
// Boost Date-Time
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// Boost Tokeniser
#include <boost/tokenizer.hpp>
// Boost Program Options
#include <boost/program_options.hpp>
// Boost Accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/config/trademgen-paths.hpp>
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandCharacteristicTypes.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/basic/RandomGeneration.hpp>
#include <trademgen/basic/RandomGenerationContext.hpp>
#include <trademgen/basic/DictionaryManager.hpp>
#include <trademgen/bom/BomManager.hpp>
#include <trademgen/config/trademgen-paths.hpp>

// Aliases for namespaces
namespace ba = boost::accumulators;

// //////// Specific type definitions ///////
typedef unsigned int NbOfRuns_T;

/** Type definition to gather statistics. */
typedef ba::accumulator_set<double,
                            ba::stats<ba::tag::min, ba::tag::max,
                                      ba::tag::mean (ba::immediate),
                                      ba::tag::sum,
                                      ba::tag::variance> > stat_acc_type;

// //////// Constants //////
/** Default name and location for the log file. */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_LOG_FILENAME ("trademgen.log");

/** Default name and location for the (CSV) input file. */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                             "/demand01.csv");

/** Default name and location for the (CSV) output file. */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_OUTPUT_FILENAME ("request.csv");

/** Default number of random draws to be generated (best if over 100). */
const NbOfRuns_T K_TRADEMGEN_DEFAULT_RANDOM_DRAWS = 100;


/** Display the statistics held by the dedicated accumulator. */
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

/** Early return status (so that it can be differentiated from an error). */
const int K_TRADEMGEN_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], NbOfRuns_T& ioRandomRuns,
                       stdair::Filename_T& ioInputFilename,
                       stdair::Filename_T& ioOutputFilename,
                       stdair::Filename_T& ioLogFilename) {

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

  if (vm.count ("input")) {
    ioInputFilename = vm["input"].as< std::string >();
  }

  if (vm.count ("output")) {
    ioOutputFilename = vm["output"].as< std::string >();
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
  }

  //
  std::cout << "The number of runs is: " << ioRandomRuns << std::endl;
  std::cout << "Input filename is: " << ioInputFilename << std::endl;
  std::cout << "Output filename is: " << ioOutputFilename << std::endl;
  std::cout << "Log filename is: " << ioLogFilename << std::endl;
  
  return 0;
}

// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  // Number of random draws to be generated (best if greater than 100)
  NbOfRuns_T lNbOfRuns;
    
  // Initialise the statistics collector/accumulator
  stat_acc_type lStatAccumulator;

  // Input file name
  stdair::Filename_T lInputFilename;

  // Output file name
  stdair::Filename_T lOutputFilename;

  // Output log File
  stdair::Filename_T lLogFilename;

  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lNbOfRuns, lInputFilename,
                       lOutputFilename, lLogFilename);

  if (lOptionParserStatus == K_TRADEMGEN_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Open and clean the .csv output file
  std::ofstream output;
  output.open (lOutputFilename.c_str());
  output.clear();
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the TraDemGen service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lInputFilename);

  // DEBUG
  //return 0;

  for (NbOfRuns_T runIdx = 1; runIdx <= lNbOfRuns; ++runIdx) {
    // /////////////////////////////////////////////////////
    output << "Run number: " << runIdx << std::endl;
    
    // Event queue
    stdair::EventQueue lEventQueue = stdair::EventQueue ();
     
    /**
       Initialisation step.
       <br>Generate the first event for each demand stream.
    */
    trademgenService.generateFirstRequests (lEventQueue);
      
    /**
       Main loop.
       <ul>
       <li>Pop a request and get its associated type/demand stream.</li>
       <li>Generate the next request for the same type/demand stream.</li>
       </ul>
    */
    stdair::Count_T eventIdx = 1;
    while (lEventQueue.isQueueDone() == false) {

      // Get the next event from the event queue
      stdair::EventStruct& lEventStruct = lEventQueue.popEvent();
      
      // Extract the corresponding demand/booking request
      const stdair::BookingRequestStruct& lPoppedRequest =
        lEventStruct.getBookingRequest();
        
      // DEBUG
      STDAIR_LOG_DEBUG ("[" << runIdx << "][" << eventIdx
                        << "] Poped booking request: '"
                        << lPoppedRequest.describe() << "'.");
    
      // Dump the request into the dedicated CSV file
      TRADEMGEN::BomManager::csvDisplay (output, lPoppedRequest);
        
      // Retrieve the corresponding demand stream
      const stdair::DemandStreamKeyStr_T& lDemandStreamKey =
        lEventStruct.getDemandStreamKey ();
      
      // Assess whether more events should be generated for that demand stream
      const bool stillHavingRequestsToBeGenerated = 
        trademgenService.stillHavingRequestsToBeGenerated (lDemandStreamKey);

      // Retrieve, from the demand stream, the total number of events
      // to be generated
      const stdair::NbOfRequests_T& lNbOfRequests = trademgenService.
        getTotalNumberOfRequestsToBeGenerated (lDemandStreamKey);
      
      // DEBUG
      STDAIR_LOG_DEBUG ("=> [" << lDemandStreamKey << "] is now processed. "
                        << "Still generate events for that demand stream? "
                        << stillHavingRequestsToBeGenerated);
    
      // If there are still events to be generated for that demand stream,
      // generate and add them to the event queue
      if (stillHavingRequestsToBeGenerated) {
        
        stdair::BookingRequestPtr_T lNextRequest_ptr =
          trademgenService.generateNextRequest (lDemandStreamKey);
        assert (lNextRequest_ptr != NULL);

        // Sanity check
        const stdair::Duration_T lDuration =
          lNextRequest_ptr->getRequestDateTime()
          - lPoppedRequest.getRequestDateTime();
        if (lDuration.total_milliseconds() < 0) {
          STDAIR_LOG_ERROR ("[" << lDemandStreamKey << "] The date-time of the "
                            << "generated event ("
                            << lNextRequest_ptr->getRequestDateTime()
                            << ") is lower than the date-time "
                            << "of the current event ("
                            << lPoppedRequest.getRequestDateTime() << ")");
          assert (false);
        }

        //
        const stdair::EventType_T lEventTypeStr ("Request");
        stdair::EventStruct lNextEventStruct (lEventTypeStr, lDemandStreamKey,
                                              lNextRequest_ptr);

        /**
           Note that when adding an event in the event queue, the
           former can be altered. It happends when an event already
           exists in the event queue with exactly the same date-time
           stamp. In that case, the date-time stamp is altered for the
           newly added event, so that the unicity on the date-time
           stamp can be guaranteed.
        */
        lEventQueue.addEvent (lNextEventStruct);

        // DEBUG
        STDAIR_LOG_DEBUG ("[" << lDemandStreamKey << "] Added request: '"
                          << lNextRequest_ptr->describe()
                          << "'. Is queue done? " << lEventQueue.isQueueDone());
      }
    
      /**
         Remove the last used event, so that, at any given moment, the
         queue keeps only the active events.
      */
      lEventQueue.eraseLastUsedEvent ();

      // Iterate
      ++eventIdx;
    }

    // Add the number of events to the statistics accumulator
    lStatAccumulator (eventIdx);
    
    // Reset the service (including the event queue) for the next run
    eventIdx = 0;
    trademgenService.reset();
  }

  // DEBUG
  std::ostringstream oStr;
  stat_display (oStr, lStatAccumulator);
  STDAIR_LOG_DEBUG (oStr.str());
  
  // Close the Log outputFile
  logOutputFile.close();

  return 0;
}
