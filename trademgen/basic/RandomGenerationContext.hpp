#ifndef __TRADEMGEN_BAS_RANDOM_GENERATION_CONTEXT_HPP
#define __TRADEMGEN_BAS_RANDOM_GENERATION_CONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_maths_types.hpp>

namespace TRADEMGEN {

  /** Structure holding the context necessary for demand random generation. */
  struct RandomGenerationContext {
    
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    RandomGenerationContext();
    /** Default constructors. */
    RandomGenerationContext (const RandomGenerationContext&);
    /** Destructor */
    ~RandomGenerationContext();
    
    // /////////////// Business Methods //////////
    /** Increment counter of requests generated so far */
    void incrementGeneratedRequestsCounter();

    /** Reset the counters. */
    void reset();

        // ////////// Attributes //////////
    /** Cumulative probability in arrival pattern for last request
        generated so far (needed for sequential generation)*/
    stdair::Probability_T _cumulativeProbabilitySoFar;
    
    /** Number of requests generated so far */
    stdair::Count_T _numberOfRequestsGeneratedSoFar;
  };

}
#endif // __STDAIR_BAS_RANDOM_GENERATION_CONTEXT_HPP
