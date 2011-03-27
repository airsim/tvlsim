// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/stdair_date_time_types.hpp>
// TraDemGen
#include <trademgen/basic/DemandDistribution.hpp>

namespace TRADEMGEN {

  // /////////////////////////////////////////////////////
  DemandDistribution::DemandDistribution (const stdair::NbOfRequests_T& iMean,
                                          const stdair::StdDevValue_T& iStdDev)
    : _meanNumberOfRequests (iMean),
      _stdDevNumberOfRequests (iStdDev) {
  }
  
  // /////////////////////////////////////////////////////
  DemandDistribution::DemandDistribution() {
  }
  
  // /////////////////////////////////////////////////////
  DemandDistribution::~DemandDistribution() {
  }
  
  // /////////////////////////////////////////////////////
  DemandDistribution::
  DemandDistribution (const DemandDistribution& iDemandDistribution)
    : _meanNumberOfRequests (iDemandDistribution._meanNumberOfRequests),
      _stdDevNumberOfRequests (iDemandDistribution._stdDevNumberOfRequests) {
  }
  
  // /////////////////////////////////////////////////////
  void DemandDistribution::fromStream (std::istream& ioIn) {
  }

  // /////////////////////////////////////////////////////
  const std::string DemandDistribution::describe() const {
    std::ostringstream oStr;
    oStr << "N (" << _meanNumberOfRequests << ", "
         << _stdDevNumberOfRequests << ")";
    return oStr.str();
  }
    
  // /////////////////////////////////////////////////////
  std::string DemandDistribution::display() const {
    std::ostringstream oStr;
    oStr << describe() << std::endl;
    return oStr.str();
  }
    
}

