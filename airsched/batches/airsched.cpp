// STL
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <airsched/AIRSCHED_Service.hpp>
#include <airsched/batches/BookingRequestParser.hpp>
#include <airsched/config/airsched-paths.hpp>

// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_AIRSCHED_DEFAULT_LOG_FILENAME ("airsched.log");

/** Default name and location for the (CSV) input file. */
const std::string K_AIRSCHED_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR "/schedule03.csv");

/** Default for the input type. It can be either built-in or provided by an
    input file. That latter must then be given with the -i option. */
const bool K_AIRSCHED_DEFAULT_BOOKING_REQUEST_MODE = false;

/** Default booking request string, to be seached against the AirSched
    network. */
const std::string K_AIRSCHED_DEFAULT_BOOKING_REQUEST ("NCE BKK NCE 2007-04-21 2007-03-21 08:32:00 C 1 DF RO 5 NONE 10:00:00 2000.0 20.0");

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
const int K_AIRSCHED_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[],
                       bool& ioReadBookingRequestFromCmdLine,
                       stdair::Filename_T& ioInputFilename,
                       std::string& ioLogFilename,
                       std::string& ioBookingRequestString) {
  // Default for the booking request mode (whether it is read from command-line)
  ioReadBookingRequestFromCmdLine = K_AIRSCHED_DEFAULT_BOOKING_REQUEST_MODE;

  //
  WordList_T lWordList;

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
     boost::program_options::value< std::string >(&ioInputFilename)->default_value(K_AIRSCHED_DEFAULT_INPUT_FILENAME),
     "(CVS) input file for the demand distributions")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_AIRSCHED_DEFAULT_LOG_FILENAME),
     "Filename for the logs")
    ("read_booking_request,r",
     "A booking request is given as a command-line option. That latter must then be given with the -b/--bkg_req option")
    ("bkg_req,b",
     boost::program_options::value< WordList_T >(&lWordList)->multitoken(),
     "Booking request word list (e.g. NCE BKK NCE 2007-04-21 2007-04-21 10:00:00 C 1 DF RO 5 NONE 10:0:0 2000.0 20.0), which sould be located at the end of the command line (otherwise, the other options would be interpreted as part of that booking request word list)")
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

  std::ifstream ifs ("airsched.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_AIRSCHED_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_AIRSCHED_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_AIRSCHED_EARLY_RETURN_STATUS;
  }

  if (vm.count ("input")) {
    ioInputFilename = vm["input"].as< std::string >();
    std::cout << "Input filename is: " << ioInputFilename << std::endl;
  }

  if (vm.count ("read_booking_request")) {
    ioReadBookingRequestFromCmdLine = true;
  }
  const std::string readBookingRequestFromCmdLineStr =
    (ioReadBookingRequestFromCmdLine == true)?"yes":"no";
  std::cout << "A booking request is to be given as command-line option? "
            << readBookingRequestFromCmdLineStr << std::endl;

  if (ioReadBookingRequestFromCmdLine == true) {

    if (lWordList.empty() == true) {
      std::cerr << "When the --read_booking_request/-r option is given, "
                << "a query must also be provided (with the --bkg_req/-b "
                << "option at the end of the command-line)" << std::endl;
      return K_AIRSCHED_EARLY_RETURN_STATUS;
    }
    
    // Rebuild the booking request query string
    ioBookingRequestString = createStringFromWordList (lWordList);
    std::cout << "The booking request string is: " << ioBookingRequestString
              << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  return 0;
}

// //////////////////////////////////////////////////////////////
stdair::BookingRequestStruct
parseBookingRequest (const std::string& iRequestOption) {
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(" -:");

  tokenizer tokens (iRequestOption, sep);

  // Origin (e.g., "NCE")
  tokenizer::iterator tok_iter = tokens.begin();
  assert (tok_iter != tokens.end());
  const stdair::AirportCode_T iOrigin (*tok_iter);

  // Destination (e.g., "BKK")
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::AirportCode_T iDestination (*tok_iter);
  
  // POS (e.g., "NCE")
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::AirportCode_T iPOS (*tok_iter);
  
  // Preferred departure date (e.g., "2007-04-21")
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lDepDateYear = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lDepDateMonth = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lDepDateDay = boost::lexical_cast<short> (*tok_iter);
  const stdair::Date_T iDepartureDate(lDepDateYear, lDepDateMonth, lDepDateDay);

  // Request date (e.g., "2007-03-21")
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lReqDateYear = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lReqDateMonth = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lReqDateDay = boost::lexical_cast<short> (*tok_iter);
  const stdair::Date_T iRequestDate (lReqDateYear, lReqDateMonth, lReqDateDay);

  // Request time (e.g., "08:34:23")
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lReqTimeHours = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lReqTimeMinutes = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lReqTimeSeconds = boost::lexical_cast<short> (*tok_iter);
  const stdair::Duration_T iRequestTime (lReqTimeHours, lReqTimeMinutes,
                                         lReqTimeSeconds);

  // Request date-time (aggregation of the two items above)
  const stdair::DateTime_T iRequestDateTime (iRequestDate, iRequestTime);
  
  // Preferred cabin (e.g., "C")
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::CabinCode_T iPreferredCabin (*tok_iter);
  
  // Party size (e.g., 1)
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::NbOfSeats_T iPartySize = 1;
  
  // Channel (e.g., "DF")
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::ChannelLabel_T iChannel (*tok_iter);
  
  // Trip type (e.g., "RO")
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::TripType_T iTripType (*tok_iter);
  
  // Stay duration (e.g., 5)
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::DayDuration_T iStayDuration = 5;
  
  // Frequent flyer (e.g., "NONE")
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::FrequentFlyer_T iFrequentFlyerType ("NONE");
  
  // Preferred departure time (e.g., "10:00:00")
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lPrefTimeHours = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lPrefTimeMinutes = boost::lexical_cast<short> (*tok_iter);
  ++tok_iter; assert (tok_iter != tokens.end());
  const short lPrefTimeSeconds = boost::lexical_cast<short> (*tok_iter);
  const stdair::Duration_T iPreferredDepartureTime (lPrefTimeHours,
                                                    lPrefTimeMinutes,
                                                    lPrefTimeSeconds);

  // Willingness-to-pay (e.g., 2000.0)
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::WTP_T iWTP = 2000.0;
  
  // Value of time (e.g., 20.0)
  ++tok_iter; assert (tok_iter != tokens.end());
  const stdair::PriceValue_T iValueOfTime = 20.0;

  // Build and return the booking request structure
  return stdair::BookingRequestStruct (iOrigin,
                                       iDestination, iPOS,
                                       iDepartureDate, iRequestDateTime,
                                       iPreferredCabin, iPartySize,
                                       iChannel, iTripType, iStayDuration,
                                       iFrequentFlyerType,
                                       iPreferredDepartureTime, iWTP,
                                       iValueOfTime);
}

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // A booking request should be given as command-line option
  bool readBookingRequestFromCmdLine;
    
  // Input file name
  stdair::Filename_T lInputFilename;

  // Output log File
  stdair::Filename_T lLogFilename;

  // Booking request string
  std::string lBookingRequestString;
    
  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, readBookingRequestFromCmdLine,
                       lInputFilename, lLogFilename, lBookingRequestString);

  if (lOptionParserStatus == K_AIRSCHED_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the AirSched service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  AIRSCHED::AIRSCHED_Service airschedService (lLogParams, lInputFilename);

  // Check wether or not a booking request is given as a command-line option
  if (readBookingRequestFromCmdLine == false) {
    lBookingRequestString = K_AIRSCHED_DEFAULT_BOOKING_REQUEST;
  }    

  // DEBUG
  STDAIR_LOG_DEBUG("Booking request string: '" << lBookingRequestString << "'");

  // Create a booking request object
  const stdair::BookingRequestStruct& lBookingRequest =
    parseBookingRequest (lBookingRequestString);

  //
  stdair::TravelSolutionList_T lTravelSolutionList;
  airschedService.buildSegmentPathList (lTravelSolutionList, lBookingRequest);
  
  // DEBUG
  STDAIR_LOG_DEBUG ("Parsed booking request: " << lBookingRequest);

  /**
     TODO: move the following display code within the StdAir library
   */
  // Browse the list of travel-solution objects
  unsigned short segmentPathIdx = 1;
  stdair::TravelSolutionList_T::const_iterator itTravelSolution =
    lTravelSolutionList.begin();
  for ( ; itTravelSolution != lTravelSolutionList.end();
        ++itTravelSolution, ++segmentPathIdx) {
    //
    const stdair::SegmentPath_T& lSegmentPath = (*itTravelSolution).getSegmentPath();

    // Dump the segment-path
    std::ostringstream oStr;
    unsigned short segmentIdx = 1;
    for (stdair::SegmentPath_T::const_iterator itSegment = lSegmentPath.begin();
         itSegment != lSegmentPath.end(); ++itSegment, ++segmentIdx) {
      //
      const std::string& lSegmentKey = *itSegment;

      if (segmentIdx != 1) {
        oStr << "; ";
      }
      oStr << lSegmentKey;
    }

    // DEBUG
    STDAIR_LOG_DEBUG ("[" << segmentPathIdx << "] " << oStr.str());
  }
    
  // Close the Log outputFile
  logOutputFile.close();

  return 0;	
}
