// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/basic/BasConst_Period_BOM.hpp>
#include <stdair/basic/BasConst_TravelSolution.hpp>
// AIRSCHED
#include <airsched/bom/SegmentPathPeriodKey.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriodKey::SegmentPathPeriodKey ()
    : _period (),
      _boardingTime (stdair::NULL_BOOST_TIME_DURATION),
      _elapsed (stdair::NULL_BOOST_TIME_DURATION),
      _nbOfAirlines (0){
  }

  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriodKey::
  SegmentPathPeriodKey (const stdair::PeriodStruct& iPeriod,
                        const stdair::Duration_T& iBoardingTime,
                        const stdair::Duration_T& iElapsedTime,
                        const DateOffsetList_T& iBoardingDateOffsetList,
                        const stdair::NbOfAirlines_T& iNbOfAirlines)
    : _period (iPeriod),
      _boardingTime (iBoardingTime),
      _elapsed (iElapsedTime),
      _boardingDateOffsetList (iBoardingDateOffsetList),
      _nbOfAirlines (iNbOfAirlines){
  }
  
  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriodKey::~SegmentPathPeriodKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathPeriodKey::toStream (std::ostream& ioOut) const {
    ioOut << "SegmentPathPeriodKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathPeriodKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string SegmentPathPeriodKey::toString() const {
    std::ostringstream oStr;
    oStr << _period.describeShort () << ", "
         << _boardingTime << ", "  << _elapsed << ", ";
    for (DateOffsetList_T::const_iterator itOffset =
           _boardingDateOffsetList.begin();
         itOffset != _boardingDateOffsetList.end(); ++itOffset) {
      const stdair::DateOffset_T& lDateOffset = *itOffset;
      oStr << lDateOffset.days() << ", ";
    }

    oStr << _nbOfAirlines ;
    return oStr.str();
  }
  
}
