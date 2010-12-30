// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
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


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_TRADEMGEN_DEFAULT_LOG_FILENAME ("trademgen.log");

/** Default name and location for the (CSV) input file. */
const std::string K_TRADEMGEN_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR "/demand01.csv");

/** Default name and location for the (CSV) output file. */
const std::string K_TRADEMGEN_DEFAULT_OUTPUT_FILENAME ("request.csv");

/** Default query string. */
const std::string K_TRADEMGEN_DEFAULT_QUERY_STRING ("my good old query");


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
const int K_TRADEMGEN_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], 
                       std::string& ioQueryString,
                       stdair::Filename_T& ioInputFilename,
                       stdair::Filename_T& ioOutputFilename,
                       std::string& ioLogFilename) {

  // Initialise the travel query string, if that one is empty
  if (ioQueryString.empty() == true) {
    ioQueryString = K_TRADEMGEN_DEFAULT_QUERY_STRING;
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
    ("input,i",
     boost::program_options::value< std::string >(&ioInputFilename)->default_value(K_TRADEMGEN_DEFAULT_INPUT_FILENAME),
     "(CVS) input file for the demand distributions")
    ("output,o",
     boost::program_options::value< std::string >(&ioOutputFilename)->default_value(K_TRADEMGEN_DEFAULT_OUTPUT_FILENAME),
     "(CVS) output file for the generated requests")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_TRADEMGEN_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
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
    std::cout << "Input filename is: " << ioInputFilename << std::endl;
  }

  if (vm.count ("output")) {
    ioOutputFilename = vm["output"].as< std::string >();
    std::cout << "Output filename is: " << ioOutputFilename << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  ioQueryString = createStringFromWordList (lWordList);
  std::cout << "The query string is: " << ioQueryString << std::endl;
  
  return 0;
}

// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  // Query
  std::string lQuery;

  // Input file name
  stdair::Filename_T lInputFilename;

  // Output file name
  stdair::Filename_T lOutputFilename;

  // Output log File
  std::string lLogFilename;

  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lQuery, lInputFilename,
                       lOutputFilename, lLogFilename);

  if (lOptionParserStatus == K_TRADEMGEN_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Number of generations.
  const int lNbOfRuns = 10;

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

  for (int i = 1; i <= lNbOfRuns; ++i) {
    // /////////////////////////////////////////////////////
    output << "Generation number " << i << std::endl;
    // Event queue
    stdair::EventQueue lEventQueue = stdair::EventQueue ();
    // Browse the list of DemandStreams and Generate the first event for each
    // DemandStream.
    trademgenService.generateFirstRequests (lEventQueue);
      
    // Pop requests, get type, and generate next request of same type
    while (lEventQueue.isQueueDone() == false) {
      stdair::EventStruct& lEventStruct = lEventQueue.popEvent ();
      const stdair::BookingRequestStruct& lPoppedRequest =
        lEventStruct.getBookingRequest ();
        
      // Output the request.
      TRADEMGEN::BomManager::csvDisplay (output, lPoppedRequest);
        
      // Retrieve the corresponding demand stream
      const stdair::DemandStreamKeyStr_T& lDemandStreamKey =
        lEventStruct.getDemandStreamKey ();
      // generate next request
      bool stillHavingRequestsToBeGenerated = 
        trademgenService.stillHavingRequestsToBeGenerated(lDemandStreamKey);
      if (stillHavingRequestsToBeGenerated) {
        stdair::BookingRequestPtr_T lNextRequest =
          trademgenService.generateNextRequest (lDemandStreamKey);
        assert (lNextRequest != NULL);

        stdair::Duration_T lDuration = lNextRequest->getRequestDateTime() -
          lPoppedRequest.getRequestDateTime();
        if (lDuration.total_milliseconds() < 0) {
          STDAIR_LOG_DEBUG (lDemandStreamKey);
          STDAIR_LOG_DEBUG (lPoppedRequest.getRequestDateTime());
          STDAIR_LOG_DEBUG (lNextRequest->getRequestDateTime());
          assert (false);
        }
          
        stdair::EventStruct lNextEventStruct ("Request", lDemandStreamKey,
                                              lNextRequest);
        lEventQueue.addEvent (lNextEventStruct);
      }
      lEventQueue.eraseLastUsedEvent ();
    }

    trademgenService.reset();
  }
  
  // Close the Log outputFile
  logOutputFile.close();

  return 0;
}
