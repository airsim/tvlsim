#ifndef __TRADEMGEN_BAS_DEMAND_DISTRIBUTION_HPP
#define __TRADEMGEN_BAS_DEMAND_DISTRIBUTION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
// TraDemGen
#include <trademgen/basic/ContinuousAttribute.hpp>

namespace TRADEMGEN {
  
  /** Class modeling the distribution of a demand type. */
  struct DemandDistribution {
    
    // ////////////// Display Support Methods //////////
    /** Display demand distribution */
    std::string display() const;

    // ////////// Constructors and destructors /////////
    /** Constructor. */
    DemandDistribution (const stdair::NbOfRequests_T& iMean,
                        const stdair::StdDevValue_T& iStdDev);
    /** Default constructor. */
    DemandDistribution ();
    /** Copy constructor. */
    DemandDistribution (const DemandDistribution&);
    /** Destructor. */
     ~DemandDistribution ();

    
    // ////////// Attributes //////////
    /** Mean number of requests */
    stdair::NbOfRequests_T _meanNumberOfRequests;
    
    /** Standard deviation of number of requests */
    stdair::StdDevValue_T _stdDevNumberOfRequests;
  };

}
#endif // __STDAIR_BAS_DEMAND_DISTRIBUTION_HPP
