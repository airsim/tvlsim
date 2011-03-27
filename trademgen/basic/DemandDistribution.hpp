#ifndef __TRADEMGEN_BAS_DEMAND_DISTRIBUTION_HPP
#define __TRADEMGEN_BAS_DEMAND_DISTRIBUTION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// TraDemGen
#include <trademgen/basic/ContinuousAttribute.hpp>

namespace TRADEMGEN {
  
  /**
   * @brief Class modeling the distribution of a demand type.
   */
  struct DemandDistribution : public stdair::StructAbstract {
  public:
    // ////////// Constructors and destructors /////////
    /**
     * Constructor.
     */
    DemandDistribution (const stdair::NbOfRequests_T& iMean,
                        const stdair::StdDevValue_T& iStdDev);
    /**
     * Default constructor.
     */
    DemandDistribution();
    /**
     * Copy constructor.
     */
    DemandDistribution (const DemandDistribution&);
    /**
     * Destructor.
     */
     ~DemandDistribution();


  public:    
    // ////////////// Display Support Methods //////////
    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);

    /**
     * Display of the structure.
     */
    const std::string describe() const;

    /**
     * Display demand distribution.
     */
    std::string display() const;


  public:    
    // ////////// Attributes //////////
    /**
     * Mean number of requests.
     */
    stdair::NbOfRequests_T _meanNumberOfRequests;
    
    /**
     * Standard deviation of number of requests.
     */
    stdair::StdDevValue_T _stdDevNumberOfRequests;
  };

}
#endif // __TRADEMGEN_BAS_DEMAND_DISTRIBUTION_HPP
