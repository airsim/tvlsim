#ifndef __TEST_RANDOM_TEST_RANDOMMANAGER_HPP
#define __TEST_RANDOM_TEST_RANDOMMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// Boost Random
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
// Boost Accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

// using namespace boost;
namespace ba = boost::accumulators;

namespace test_random {

  // ////////// Type definitions //////////
  /** Type definition for the amount of random numbers to be generated. */
  typedef unsigned int amount_of_draws_type;
  
  /** Type definition for the random generator seed.
      <br>That seed must be unsigned, otherwise the wrong overload may be
      selected when using mt19937 as the base_generator_type. */
  typedef unsigned int random_generator_seed_type;

  /** Type definition to gather statistics. */
  typedef ba::accumulator_set<double,
                              ba::stats<ba::tag::min, ba::tag::max,
                                        ba::tag::mean (ba::immediate),
                                        ba::tag::sum,
                                        ba::tag::variance> > stat_acc_type;

  
  /** Helper class for testing random number generation
      and order statistics. */
  class RandomManager {
  private:
    // ////////// Type definitions //////////
    /** Type definition for a random number generator base (mt19937). */
    typedef boost::minstd_rand base_generator_type;
  
    /** Type definition for the uniform distribution random generator. */
    typedef boost::variate_generator<base_generator_type&, 
                                     boost::uniform_real<> > uni_gen_type;
    
  public:
    // /////////// Constructors and destructors /////////
    /** Constructor.
     <br>The internal random number generator is initialised with a reproducible
     seed. That seed must be unsigned, otherwise the wrong overload may be
     selected when using mt19937 as the base_generator_type. */
    RandomManager (std::ostream&, const amount_of_draws_type&,
                   const random_generator_seed_type&);
    
    /** Constructor. */
    ~RandomManager();
    
    /** Display the statistics held by the dedicated accumulator. */
    void stat_display (const stat_acc_type& iStatAcc,
                       const std::string& iTypeStr = "");

    /** */
    stat_acc_type pristine_uniform();

    /** */
    stat_acc_type sequential_dummy();

    /** */
    stat_acc_type sequential_uniform();

  private:
    // /////////// Constructors and destructors /////////
    /** Default constructors.
        <br>They are kept private so that only the public constructor
        may be used. */
    RandomManager ();
    RandomManager (const RandomManager&);

    /** Initialise the random generator. */
    void init();
  
  private:
    // /////////// Attributes ///////////
    /** Log stream. */
    std::ostream& _oStr;
    
    /** Amount of random numbers to be generated. */
    const amount_of_draws_type _nbOfDraws;

    /** Seed of the random generator. */
    const random_generator_seed_type _seed;

    /** Base for the random generator. */
    base_generator_type _generator;
    
    /** Random generator for the uniform distribution on [0,1). */
    uni_gen_type _uniformDistributionGenerator;
  };

}
#endif // __TEST_RANDOM_TEST_RANDOMMANAGER_HPP
