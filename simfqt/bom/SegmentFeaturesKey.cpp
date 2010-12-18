// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ostream>
#include <sstream>
// STDAIR
#include <simfqt/bom/SegmentFeaturesKey.hpp>

namespace SIMFQT  {

  // ////////////////////////////////////////////////////////////////////
  SegmentFeaturesKey::SegmentFeaturesKey (const stdair::AirlineCode_T& iAirlineCode,
					  const stdair::ClassCode_T& iClassCode)
    : _airlineCode (iAirlineCode), _classCode (iClassCode) {
  }

  // ////////////////////////////////////////////////////////////////////
  SegmentFeaturesKey::SegmentFeaturesKey (const SegmentFeaturesKey& iKey)
    : _airlineCode (iKey._airlineCode), _classCode (iKey._classCode) {
  }

  // ////////////////////////////////////////////////////////////////////
  SegmentFeaturesKey::~SegmentFeaturesKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentFeaturesKey::toStream (std::ostream& ioOut) const {
    ioOut << "SegmentFeaturesKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentFeaturesKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string SegmentFeaturesKey::toString() const {
    std::ostringstream oStr;
    oStr << _airlineCode << "-" << _classCode  << std::endl; 
    return oStr.str();
  }

}
