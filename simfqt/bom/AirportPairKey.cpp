// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ostream>
#include <sstream>
// STDAIR
#include <simfqt/bom/AirportPairKey.hpp>

namespace SIMFQT  {

  // ////////////////////////////////////////////////////////////////////
  AirportPairKey::AirportPairKey (const stdair::AirportCode_T& iBoardingPoint,
                                  const stdair::AirportCode_T& iOffPoint)
    : _boardingPoint (iBoardingPoint), _offPoint (iOffPoint) {
  }

  // ////////////////////////////////////////////////////////////////////
  AirportPairKey::AirportPairKey (const AirportPairKey& iKey)
    : _boardingPoint (iKey._boardingPoint), _offPoint (iKey._offPoint) {
  }

  // ////////////////////////////////////////////////////////////////////
  AirportPairKey::~AirportPairKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void AirportPairKey::toStream (std::ostream& ioOut) const {
    ioOut << "AirportPairKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void AirportPairKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string AirportPairKey::toString() const {
    std::ostringstream oStr;
    oStr << _boardingPoint << "-" << _offPoint << std::endl; 
    return oStr.str();
  }

}
