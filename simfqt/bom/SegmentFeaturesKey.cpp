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
  SegmentFeaturesKey::SegmentFeaturesKey (const stdair::AirlineCodeList_T& iAirlineCodeList,
					  const stdair::ClassList_StringList_T& iClassCodeList)
    : _airlineCodeList (iAirlineCodeList), _classCodeList (iClassCodeList) {
  }

  // ////////////////////////////////////////////////////////////////////
  SegmentFeaturesKey::SegmentFeaturesKey (const SegmentFeaturesKey& iKey)
    : _airlineCodeList (iKey._airlineCodeList), _classCodeList (iKey._classCodeList) {
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
    assert(_airlineCodeList.size() == _classCodeList.size());
    stdair::AirlineCodeList_T::const_iterator lItAirlineCode =
      _airlineCodeList.begin();
    for (stdair::ClassList_StringList_T::const_iterator lItClassCode =
           _classCodeList.begin();
         lItClassCode != _classCodeList.end();
         lItClassCode++) {
      oStr << *lItAirlineCode << " " << *lItClassCode << ",  ";
      lItAirlineCode++;
    }
    oStr << std::endl; 
    return oStr.str();
  }

}
