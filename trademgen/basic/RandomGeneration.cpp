// STL
#include <cassert>
#include <iostream>
// Boost
#include <boost/version.hpp>
#if BOOST_VERSION >= 103500
#include <boost/math/distributions/normal.hpp>
#endif // BOOST_VERSION >= 103500
//TRADEMGEN
#include <trademgen/basic/RandomGeneration.hpp>

namespace TRADEMGEN {
  
  // //////////////////////////////////////////////////////////////////////
  RandomGeneration::RandomGeneration (const stdair::RandomSeed_T& iSeed)
    : _seed (iSeed), _generator (iSeed),
      _uniformGenerator (_generator, boost::uniform_real<> (0, 1)) {
    init ();
  }

  // //////////////////////////////////////////////////////////////////////
  RandomGeneration::RandomGeneration (const RandomGeneration& iRandomGeneration)
    : _seed (iRandomGeneration._seed),
      _generator (iRandomGeneration._generator),
      _uniformGenerator (iRandomGeneration._uniformGenerator) {
  }

  // //////////////////////////////////////////////////////////////////////
  RandomGeneration::RandomGeneration ()
    : _seed (0), _generator (0),
      _uniformGenerator (_generator, boost::uniform_real<> (0, 1)) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  RandomGeneration::~RandomGeneration () {
  }

  // //////////////////////////////////////////////////////////////////////
  void RandomGeneration::init () {
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::RealNumber_T RandomGeneration::generateUniform01 () {
    const stdair::RealNumber_T lVariateUnif = _uniformGenerator();
    return lVariateUnif;
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::RealNumber_T RandomGeneration::
  generateUniform (const stdair::RealNumber_T& iMinValue,
                   const stdair::RealNumber_T& iMaxValue){
    const stdair::RealNumber_T lVariateUnif = 
      iMinValue + _uniformGenerator() * (iMaxValue - iMinValue);
    return lVariateUnif;
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::RealNumber_T RandomGeneration::
  generateNormal (const stdair::RealNumber_T& mu,
                  const stdair::RealNumber_T& sigma) {
#if BOOST_VERSION >= 103500
    const stdair::Probability_T lVariateUnif = generateUniform01 ();
    const boost::math::normal lNormal (mu, sigma);
    const stdair::RealNumber_T lRealNumberOfRequestsToBeGenerated =
      boost::math::quantile(lNormal, lVariateUnif);
#else // BOOST_VERSION >= 103500
    // TODO: rely on GSL when Boost version smaller than 1.35
    const stdair::RealNumber_T lRealNumberOfRequestsToBeGenerated = 0.0;
#endif // BOOST_VERSION >= 103500

    return lRealNumberOfRequestsToBeGenerated;
  }
}
