// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost.Serialization
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
// StdAir
#include <stdair/basic/BasConst_Inventory.hpp>
// AirSched
#include <airsched/bom/ReachableUniverse.hpp>
#include <airsched/bom/SegmentPathPeriod.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverse::ReachableUniverse()
    : _key (stdair::DEFAULT_ORIGIN), _parent (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverse::ReachableUniverse (const ReachableUniverse&)
    : _key (stdair::DEFAULT_ORIGIN), _parent (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverse::ReachableUniverse (const Key_T& iKey)
    : _key (iKey), _parent (NULL) {
  }

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverse::~ReachableUniverse() {
  }

  // ////////////////////////////////////////////////////////////////////
  std::string ReachableUniverse::toString() const {
    std::ostringstream oStr;
    oStr << _key.toString();
    return oStr.str();
  }
  
  // ////////////////////////////////////////////////////////////////////
  void ReachableUniverse::serialisationImplementation() {
    std::ostringstream oStr;
    boost::archive::text_oarchive oa (oStr);
    oa << *this;

    std::istringstream iStr;
    boost::archive::text_iarchive ia (iStr);
    ia >> *this;
  }

  // ////////////////////////////////////////////////////////////////////
  template<class Archive>
  void ReachableUniverse::serialize (Archive& ioArchive,
                                     const unsigned int iFileVersion) {
    ioArchive & _key;
  }

}

