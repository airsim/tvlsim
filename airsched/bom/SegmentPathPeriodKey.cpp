// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
// Boost.Serialization
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/basic/BasConst_Period_BOM.hpp>
#include <stdair/basic/BasConst_TravelSolution.hpp>
// AirSched
#include <airsched/bom/SegmentPathPeriodKey.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriodKey::SegmentPathPeriodKey()
    : _period (stdair::BOOST_DEFAULT_DATE_PERIOD, stdair::DEFAULT_DOW_STRING),
      _boardingTime (stdair::NULL_BOOST_TIME_DURATION),
      _elapsed (stdair::NULL_BOOST_TIME_DURATION),
      _nbOfAirlines (stdair::DEFAULT_NBOFAIRLINES) {
  }

  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriodKey::SegmentPathPeriodKey (const SegmentPathPeriodKey& iSPPK)
    : _period (iSPPK._period),
      _boardingTime (iSPPK._boardingTime),
      _elapsed (iSPPK._elapsed),
      _boardingDateOffsetList (iSPPK._boardingDateOffsetList),
      _nbOfAirlines (iSPPK._nbOfAirlines) {
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
      _nbOfAirlines (iNbOfAirlines) {
  }
  
  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriodKey::~SegmentPathPeriodKey() {
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
    oStr << _period.describeShort() << ", "
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
  
  // ////////////////////////////////////////////////////////////////////
  void SegmentPathPeriodKey::serialisationImplementation() {
    std::ostringstream oStr;
    boost::archive::text_oarchive oa (oStr);
    oa << *this;

    std::istringstream iStr;
    boost::archive::text_iarchive ia (iStr);
    ia >> *this;
  }

  // ////////////////////////////////////////////////////////////////////
  template<class Archive>
  void SegmentPathPeriodKey::serialize (Archive& ioArchive,
                                        const unsigned int iFileVersion) {
    /**
     * \note The serialised members should not be const (as, as far as
     *       I understand, they are tracked by Boost.Serialisation).
     */
    //ioArchive & _period & _boardingTime & _elapsed & _nbOfAirlines;
    std::string lBTStr = boost::posix_time::to_simple_string (_boardingTime);
    std::string lElapsedStr = boost::posix_time::to_simple_string (_elapsed);
    ioArchive & lBTStr & lElapsedStr & _nbOfAirlines;
  }

}
