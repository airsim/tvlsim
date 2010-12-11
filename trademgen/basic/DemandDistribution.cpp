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
      _stdDevNumberOfRequests (iStdDev){
  }
  
  // /////////////////////////////////////////////////////
  DemandDistribution::DemandDistribution () {
  }
  
  // /////////////////////////////////////////////////////
  DemandDistribution::~DemandDistribution () {
  }
  
  // /////////////////////////////////////////////////////
  DemandDistribution::
  DemandDistribution (const DemandDistribution& iDemandDistribution)
    : _meanNumberOfRequests (iDemandDistribution._meanNumberOfRequests),
      _stdDevNumberOfRequests (iDemandDistribution._stdDevNumberOfRequests) {
  }
  
  // /////////////////////////////////////////////////////
  std::string DemandDistribution::display() const {
    std::ostringstream oStr;

    oStr << "****************** Demand distribution ******************"
         << std::endl;
    oStr << "Mean number of requests .......... : "
         << _meanNumberOfRequests << std::endl;
    oStr << "Std dev of number of requests .... : "
         << _stdDevNumberOfRequests << std::endl;

    return oStr.str();
  }
    
}

