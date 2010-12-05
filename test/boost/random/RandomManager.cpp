// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Test Random
#include <test/boost/random/RandomManager.hpp>

namespace test_random {

  // //////////////////////////////////////////////////////////////////////
  void gen_save (std::string& ioState, const boost::minstd_rand& iGenerator) {
    std::ostringstream oStr;
    oStr << iGenerator;
    ioState = oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void gen_restore (const std::string& iState,
                    boost::minstd_rand& ioGenerator) {
    std::istringstream iStr (iState);
    iStr >> ioGenerator;
  }

  // //////////////////////////////////////////////////////////////////////
  RandomManager::RandomManager (std::ostream& ioStream,
                                const amount_of_draws_type& iNbOfDraws,
                                const random_generator_seed_type& iSeed)
    : _oStr (ioStream), _nbOfDraws (iNbOfDraws), _seed (iSeed),
      _generator (iSeed),
      _uniformDistributionGenerator (_generator,
                                     boost::uniform_real<> (0, 1)) {
    //
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void RandomManager::init() {
  }

  // //////////////////////////////////////////////////////////////////////
  RandomManager::~RandomManager () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  void RandomManager::stat_display (const stat_acc_type& iStatAcc,
                                    const std::string& iTypeStr) {
    // Store current formatting flags of _oStr
    std::ios::fmtflags oldFlags = _oStr.flags();

    //
    _oStr.setf (std::ios::fixed);

    //
    _oStr << "Statistics for the random draws (" << iTypeStr << "): "
              << std::endl;
    _oStr << "  minimum   = " << ba::min (iStatAcc) << std::endl;
    _oStr << "  mean      = " << ba::mean (iStatAcc) << std::endl;
    _oStr << "  maximum   = " << ba::max (iStatAcc) << std::endl;
    _oStr << "  count     = " << ba::count (iStatAcc) << std::endl;
    _oStr << "  variance  = " << ba::variance (iStatAcc) << std::endl;
  
    // Reset formatting flags of _oStr
    _oStr.flags (oldFlags);
  }

  // //////////////////////////////////////////////////////////////////////
  stat_acc_type RandomManager::pristine_uniform() {
    stat_acc_type oStatAccumulator;
    
    // Store current formatting flags of _oStr
    std::ios::fmtflags oldFlags = _oStr.flags();

    //
    _oStr.setf (std::ios::fixed);

    // Draw randomly
    for (unsigned int i = 1; i <= _nbOfDraws; ++i) {
      // Draw a random number corresponding to the uniform [0,1) range
      const double lDrawnNumber = _uniformDistributionGenerator();

      // Update the statistics
      oStatAccumulator (lDrawnNumber);

      // DEBUG
      // _oStr << "[" << i << "]: " << lDrawnNumber << std::endl;
    }
  
    // Reset formatting flags of _oStr
    _oStr.flags (oldFlags);

    return oStatAccumulator;
  }

  // //////////////////////////////////////////////////////////////////////
  stat_acc_type RandomManager::sequential_dummy() {
    stat_acc_type oStatAccumulator;
    
    // Store current formatting flags of _oStr
    std::ios::fmtflags oldFlags = _oStr.flags();

    //
    _oStr.setf (std::ios::fixed);

    // Draw randomly
    double lLowerBound = 0.0;
    for (unsigned int i = 1; i <= _nbOfDraws; ++i) {
      // Draw a random number corresponding to the uniform [0,1) range
      double lDrawnNumber = _uniformDistributionGenerator();

      // Transform the drawn number so that it corresponds to the uniform [x,1)
      // range distribution
      lDrawnNumber = lLowerBound + lDrawnNumber * (1 - lLowerBound);

      // Update the statistics
      oStatAccumulator (lDrawnNumber);

      // DEBUG
      // _oStr << "[" << i << "]: " << lDrawnNumber << std::endl;

      // The lower bound is translated to the latest drawn number
      lLowerBound = lDrawnNumber;
    }
  
    // Reset formatting flags of _oStr
    _oStr.flags (oldFlags);

    return oStatAccumulator;
  }

  // //////////////////////////////////////////////////////////////////////
  stat_acc_type RandomManager::sequential_uniform() {
    stat_acc_type oStatAccumulator;
    
    // Store current formatting flags of _oStr
    std::ios::fmtflags oldFlags = _oStr.flags();

    //
    _oStr.setf (std::ios::fixed);

    // Draw randomly
    const double lUniformRangeLength = 1.0 / _nbOfDraws;
    double lLowerBound = 0.0;
    for (unsigned int i = 1; i <= _nbOfDraws; ++i) {
      // Draw a random number corresponding to the uniform [0,1) range
      double lDrawnNumber = _uniformDistributionGenerator();

      /**
         Transform the drawn number so that it corresponds to the
         uniform [x,x+d) range distribution, where:<br>
         <ul>
         <li>d   == 1/<total number of numbers to be drawn></li>
         <li>x   == (i-1)*d</li>
         <li>x+d == i*d (by construction)</li>
         </ul>
      */
      lDrawnNumber = lUniformRangeLength * (i-1 + lDrawnNumber);

      // Update the statistics
      oStatAccumulator (lDrawnNumber);

      // DEBUG
      // _oStr << "[" << i << "]: " << lDrawnNumber << std::endl;

      // The lower bound is translated to the latest drawn number
      lLowerBound = lDrawnNumber;
    }
  
    // Reset formatting flags of _oStr
    _oStr.flags (oldFlags);

    return oStatAccumulator;
  }

}
