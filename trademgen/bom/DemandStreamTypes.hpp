#ifndef __TRADEMGEN_BOM_DEMANDSTREAMTYPES_HPP
#define __TRADEMGEN_BOM_DEMANDSTREAMTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/bom/key_types.hpp>

namespace TRADEMGEN {

  // Forward declarations.
  class DemandStream;
  
  /** Define the airline feature list. */
  typedef std::list<DemandStream*> DemandStreamList_T;

  /** Define the airline feature map. */
  typedef std::map<const stdair::MapKey_T, DemandStream*> DemandStreamMap_T;
  
}
#endif // __TRADEMGEN_BOM_DEMANDSTREAMTYPES_HPP
