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
#include <airsched/bom/OriginDestinationSet.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSet::OriginDestinationSet()
    : _key (stdair::DEFAULT_ORIGIN), _parent (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSet::OriginDestinationSet (const OriginDestinationSet&)
    : _key (stdair::DEFAULT_ORIGIN), _parent (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSet::OriginDestinationSet (const Key_T& iKey)
    : _key (iKey), _parent (NULL) {
  }

  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSet::~OriginDestinationSet() {
  }

  // ////////////////////////////////////////////////////////////////////
  std::string OriginDestinationSet::toString() const {
    std::ostringstream oStr;
    oStr << _key.toString();
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  void OriginDestinationSet::serialisationImplementation() {
    std::ostringstream oStr;
    boost::archive::text_oarchive oa (oStr);
    oa << *this;

    std::istringstream iStr;
    boost::archive::text_iarchive ia (iStr);
    ia >> *this;
  }

  // ////////////////////////////////////////////////////////////////////
  template<class Archive>
  void OriginDestinationSet::serialize (Archive& ioArchive,
                                        const unsigned int iFileVersion) {
    ioArchive & _key;
  }

  // ////////////////////////////////////////////////////////////////////
  // Explicit template instantiation
  namespace ba = boost::archive;
  template
  void OriginDestinationSet::serialize<ba::text_oarchive> (ba::text_oarchive&,
                                                           unsigned int);
  template
  void OriginDestinationSet::serialize<ba::text_iarchive> (ba::text_iarchive&,
                                                           unsigned int);
  // ////////////////////////////////////////////////////////////////////

}

