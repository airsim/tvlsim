#ifndef __TRADEMGEN_BAS_RANDOM_GENERATION_CONTEXT_HPP
#define __TRADEMGEN_BAS_RANDOM_GENERATION_CONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/basic/StructAbstract.hpp>

namespace TRADEMGEN {

  /**
   * Structure holding the context necessary for demand random generation.
   */
  struct RandomGenerationContext : public stdair::StructAbstract {
  public:
    // ////////// Getters /////////
    /**
     * Get the number of requests generated so far.
     */
    const stdair::Count_T& getNumberOfRequestsGeneratedSoFar() const {
      return _numberOfRequestsGeneratedSoFar;
    }

    /**
     * Get the cumulative probability in arrival pattern for last
     * request generated so far (needed for sequential generation).
     */
    const stdair::Probability_T& getCumulativeProbabilitySoFar() const {
      return _cumulativeProbabilitySoFar;
    }
    
  public:
    // ////////// Setters /////////
    /**
     * Set the number of requests generated so far.
     */
    void setNumberOfRequestsGeneratedSoFar (const stdair::Count_T& iCount) {
      _numberOfRequestsGeneratedSoFar = iCount;
    }

    /**
     * Set the cumulative probability in arrival pattern for last
     * request generated so far (needed for sequential generation).
     */
    void setCumulativeProbabilitySoFar (const stdair::Probability_T& iProb) {
      _cumulativeProbabilitySoFar = iProb;
    }
    

  public: 
    // ////////// Constructors and destructors /////////
    /**
     * Default constructor.
     */
    RandomGenerationContext();

    /**
     * Default constructors.
     */
    RandomGenerationContext (const RandomGenerationContext&);

    /**
     * Destructor.
     */
    ~RandomGenerationContext();
    

  public:
    // /////////////// Business Methods //////////
    /**
     * Increment counter of requests generated so far.
     */
    void incrementGeneratedRequestsCounter();

    /**
     * Reset the counters.
     */
    void reset();


  public:
    // ////////////// Display Support Methods //////////
    /**
     * Give a description of the structure (for display purposes).
     */
    const std::string describe() const;


  private:
    // ////////// Attributes //////////
    /**
     * Number of requests generated so far.
     */
    stdair::Count_T _numberOfRequestsGeneratedSoFar;

    /**
     * Cumulative probability in arrival pattern for last request
     * generated so far (needed for sequential generation).
     */
    stdair::Probability_T _cumulativeProbabilitySoFar;   
  };

}
#endif // __STDAIR_BAS_RANDOM_GENERATION_CONTEXT_HPP
