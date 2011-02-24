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
					  const std::list<std::string>& iClassCodeList)
    : _airlineCode (iAirlineCode), _classCodeList (iClassCodeList) {
  }

  // ////////////////////////////////////////////////////////////////////
  SegmentFeaturesKey::SegmentFeaturesKey (const SegmentFeaturesKey& iKey)
    : _airlineCode (iKey._airlineCode), _classCodeList (iKey._classCodeList) {
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
    oStr << _airlineCode << "-";
    for (std::list<std::string>::const_iterator lItClassCode =
           _classCodeList.begin();
         lItClassCode != _classCodeList.end();
         lItClassCode++) {
      oStr << *lItClassCode << " ";
    }
    oStr << std::endl; 
    return oStr.str();
  }

}
