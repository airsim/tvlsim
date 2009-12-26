// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/program_options.hpp>

// Constants
const int K_JUST_INFO_QUERY = 99;
const std::string K_LOGD_VERSION_STRING ("Log server/daemon (logd), version 0.01");
const std::string K_LOGD_DEFAULT_LOG_FILEPATH ("log/logd.log");

// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

// /////////////////////////////////////////////////////
int readConfiguration (int argc, char* argv[], std::string& ioLogFilepath) {
  // Declare a group of options that will be 
  // allowed only on command line
  boost::program_options::options_description lHelper("Helper");
  lHelper.add_options()
    ("version,v", "print version string")
    ("help,h", "produce help message");
    
  // Declare a group of options that will be 
  // allowed both on command line and in
  // config file
  boost::program_options::options_description lOptions("Options");
  std::string lLogFilePath;
  lOptions.add_options()
    ("log,l", 
     boost::program_options::value< std::string >(&lLogFilePath)->default_value(K_LOGD_DEFAULT_LOG_FILEPATH), 
     "full path of the log file");

  boost::program_options::options_description cmdline_options;
  cmdline_options.add(lHelper).add(lOptions);

  boost::program_options::options_description config_file_options;
  config_file_options.add(lOptions);

  boost::program_options::options_description lVisible("Allowed options");
  lVisible.add(lHelper).add(lOptions);
        
  boost::program_options::variables_map vm;
  boost::program_options::
    store (boost::program_options::command_line_parser(argc, argv).
           options (cmdline_options).run(), vm);

  std::ifstream ifs ("logd.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("version")) {
    std::cout << K_LOGD_VERSION_STRING << std::endl;
    return K_JUST_INFO_QUERY;
  }

  if (vm.count ("help")) {
    std::cout << lVisible << std::endl;
    return K_JUST_INFO_QUERY;
  }

  ioLogFilepath = vm["log"].as< std::string >();
  if (vm.count ("log")) {
    std::cout << "Log file is: " << ioLogFilepath << std::endl;
  }

  return 0;
}


// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  // Output log File
  std::string lLogFilename ("log/logd.log");
    
  const int hasConfBeenRead = readConfiguration (argc, argv, lLogFilename);
  if (hasConfBeenRead == K_JUST_INFO_QUERY) {
    return 0;
  }
  
  try {
    
    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();

    logOutputFile << "The logd service is running..." << std::endl;
    
    // Close the Log outputFile
    logOutputFile.close();

  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return -1;
    
  } catch (...) {
    std::cerr << "Unknown exception: " << std::endl;
    return -1;
  }
  
  return 0;
}
