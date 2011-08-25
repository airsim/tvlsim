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
#include <airsched/bom/ReachableUniverseKey.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverseKey::ReachableUniverseKey()
    : _origin (stdair::DEFAULT_ORIGIN) {
    assert (false);
  }
  
  // ////////////////////////////////////////////////////////////////////
  ReachableUniverseKey::
  ReachableUniverseKey (const ReachableUniverseKey& iKey)
    : _origin (iKey._origin) {
  }

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverseKey::
  ReachableUniverseKey (const stdair::AirportCode_T& iAirportCode)
    : _origin (iAirportCode) {
  }

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverseKey::~ReachableUniverseKey() {
  }

  // ////////////////////////////////////////////////////////////////////
  void ReachableUniverseKey::toStream (std::ostream& ioOut) const {
    ioOut << "ReachableUniverseKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void ReachableUniverseKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string ReachableUniverseKey::toString() const {
    std::ostringstream oStr;
    oStr << _origin;
    return oStr.str();
  }
  
  // ////////////////////////////////////////////////////////////////////
  void ReachableUniverseKey::serialisationImplementation() {
    std::ostringstream oStr;
    boost::archive::text_oarchive oa (oStr);
    oa << *this;

    std::istringstream iStr;
    boost::archive::text_iarchive ia (iStr);
    ia >> *this;
  }

  // ////////////////////////////////////////////////////////////////////
  template<class Archive>
  void ReachableUniverseKey::serialize (Archive& ioArchive,
                                        const unsigned int iFileVersion) {
    /**
     * \note The serialised members should not be const (as, as far as
     *       I understand, they are tracked by Boost.Serialisation).
     */
    ioArchive & _origin;
  }

  // ////////////////////////////////////////////////////////////////////
  // Explicit template instantiation
  namespace ba = boost::archive;
  template
  void ReachableUniverseKey::serialize<ba::text_oarchive> (ba::text_oarchive&,
                                                           unsigned int);
  template
  void ReachableUniverseKey::serialize<ba::text_iarchive> (ba::text_iarchive&,
                                                           unsigned int);
  // ////////////////////////////////////////////////////////////////////
  
}
