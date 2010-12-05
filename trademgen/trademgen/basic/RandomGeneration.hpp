#ifndef __TRADEMGEN_BAS_BOM_RANDOMGENERATION_HPP
#define __TRADEMGEN_BAS_BOM_RANDOMGENERATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost Random
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_maths_types.hpp>

namespace TRADEMGEN {

  /** Random generator. */
  struct RandomGeneration {
    // //////////// Business Methods /////////////
    /** Generate a randomized number following a uniform distribution
        between 0 (included) and 1 (excluded). */
    stdair::RealNumber_T generateUniform01 ();
    
    /** Generate a randomized number following a uniform distribution
        between a minimum (included) and a maximum (excluded)
        value. */
    stdair::RealNumber_T generateUniform (const stdair::RealNumber_T&,
                                          const stdair::RealNumber_T&);
    
    /** Generate a randomized number following a normal distribution
        specified by a mean and a standard deviation. */
    stdair::RealNumber_T generateNormal (const stdair::RealNumber_T&,
                                         const stdair::RealNumber_T&);
    
    // ////////// Constructors and destructors /////////
    /** Constructor. */
    RandomGeneration (const stdair::RandomSeed_T&);
    /** Default constructors. */
    RandomGeneration ();
    RandomGeneration (const RandomGeneration&);
    /** Destructor. */
    ~RandomGeneration ();
    /** Initialize the random generator.
        <br>A uniform random number distribution is defined, which
        produces "real" values between 0 and 1 (0 inclusive, 1
        exclusive). */
    void init ();

    // ////////// Attributes //////////
    /** The seed of the random generator.
        <br>The seed is unsigned, otherwise the wrong overload may be
        selected when using mt19937 as the boost::base_generator_type. */
    stdair::RandomSeed_T _seed;

    /** Random number generator engine.
        <br>The random number generator is currently based on boost::minstd_rand.
        Alternates are boost::mt19937, boost::ecuyer1988. */
    stdair::BaseGenerator_T _generator;
    
    /** Random number generator.
        <br>It is initialized with a reproducible seed and a uniform
        distribution. */
    boost::variate_generator<stdair::BaseGenerator_T&,
                             boost::uniform_real<> > _uniformGenerator;
  };

}
#endif // __TRADEMGEN_BAS_BOM_RANDOMGENERATION_HPP
