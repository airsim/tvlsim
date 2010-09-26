// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
// Boost Program Options
#include <boost/program_options.hpp>
// Local
#include <test/boost/random/RandomManager.hpp>

// //////// Constants ////////
/** Default name and location for the log file. */
const std::string K_RANDOM_DEFAULT_LOG_FILENAME ("random_order_stats.log");

/** Default amount of random numbers to be generated. */
const test_random::amount_of_draws_type K_RANDOM_DEFAULT_NB_OF_DRAWS = 10000;

/** Default seed for the random generator. */
const test_random::random_generator_seed_type K_RANDOM_DEFAULT_GENERATOR_SEED = 42u;

/** Early return status (so that it can be differentiated from an
    error). */
const int K_RANDOM_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[],
                       test_random::amount_of_draws_type& ioAmountOfDraws,
                       test_random::random_generator_seed_type& ioRandomGeneratorSeed,
                       std::string& ioLogFilename) {
  // Declare a group of options that will be allowed only on command line
  boost::program_options::options_description generic ("Generic options");
  generic.add_options()
    ("version,v", "print version string")
    ("help,h", "produce help message");
    
  // Declare a group of options that will be allowed both on command
  // line and in config file
  boost::program_options::options_description config ("Configuration");
  config.add_options()
    ("draws,n",
     boost::program_options::value< test_random::amount_of_draws_type >(&ioAmountOfDraws)->default_value(K_RANDOM_DEFAULT_NB_OF_DRAWS),
     "Amount of random numbers to be generated")
    ("seed,s",
     boost::program_options::value< test_random::random_generator_seed_type >(&ioRandomGeneratorSeed)->default_value(K_RANDOM_DEFAULT_GENERATOR_SEED),
     "Seed for the random generator")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_RANDOM_DEFAULT_LOG_FILENAME),
     "Filename for the logs")
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

  std::ifstream ifs ("random_order_stats.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_RANDOM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << "Order Statistics Helper Tool, version 1.0" << std::endl;
    return K_RANDOM_EARLY_RETURN_STATUS;
  }

  if (vm.count ("draws")) {
    ioAmountOfDraws = vm["draws"].as< test_random::amount_of_draws_type >();
    std::cout << "Amount of random numbers to be generated: " << ioAmountOfDraws
              << std::endl;
  }

  if (vm.count ("seed")) {
    ioRandomGeneratorSeed = vm["seed"].as< test_random::random_generator_seed_type >();
    std::cout << "Random generator seed: "<< ioRandomGeneratorSeed << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  return 0;
}


// //////////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

  /** Amount of random numbers to be generated. */
  test_random::amount_of_draws_type lAmountOfDraws;

  /** Seed for the random generator. */
  test_random::random_generator_seed_type lRandomGeneratorSeed;
  
  // Output log File
  std::string lLogFilename;
    
  // Call the command-line option parser
  const int lOptionParserStatus =
    readConfiguration (argc, argv, lAmountOfDraws, lRandomGeneratorSeed,
                       lLogFilename);
  
  if (lOptionParserStatus == K_RANDOM_EARLY_RETURN_STATUS) {
    return 0;
  }
  
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the RandomManager service object
  test_random::RandomManager lRandomManager (logOutputFile, lAmountOfDraws,
                                             lRandomGeneratorSeed);

  // Pristine uniform method
  const test_random::stat_acc_type& lPristineUniformStatAcc =
    lRandomManager.pristine_uniform();
  lRandomManager.stat_display (lPristineUniformStatAcc,
                               "pristine uniform [0,1)");
  
  // Dummy sequential method
  const test_random::stat_acc_type& lDummySequentialStatAcc =
    lRandomManager.sequential_dummy();
  lRandomManager.stat_display (lDummySequentialStatAcc, "dummy sequential");
  
  // Sequential uniform method
  const test_random::stat_acc_type& lSequentialUniformStatAcc =
    lRandomManager.sequential_uniform();
  lRandomManager.stat_display (lSequentialUniformStatAcc, "sequential uniform");
  
  return 0;
}
