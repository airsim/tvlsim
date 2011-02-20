// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
// AirSched
#include <airsched/bom/OriginDestinationSetKey.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSetKey::
  OriginDestinationSetKey (const stdair::AirportCode_T& iDestination)
    : _destination (iDestination) {
  }
  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSetKey::
  OriginDestinationSetKey (const OriginDestinationSetKey& iKey)
    : _destination (iKey._destination) {
  }

  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSetKey::~OriginDestinationSetKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void OriginDestinationSetKey::toStream (std::ostream& ioOut) const {
    ioOut << "OriginDestinationSetKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void OriginDestinationSetKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string OriginDestinationSetKey::toString() const {
    std::ostringstream oStr;
    oStr << _destination;
    return oStr.str();
  }

}
