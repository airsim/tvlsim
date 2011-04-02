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
#include <stdair/basic/BasConst_Inventory.hpp>
// AirSched
#include <airsched/bom/OriginDestinationSetKey.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  OriginDestinationSetKey::OriginDestinationSetKey()
    : _destination (stdair::DEFAULT_DESTINATION) {
    assert (false);
  }

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
  OriginDestinationSetKey::~OriginDestinationSetKey() {
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

  // ////////////////////////////////////////////////////////////////////
  void OriginDestinationSetKey::serialisationImplementation() {
    std::ostringstream oStr;
    boost::archive::text_oarchive oa (oStr);
    oa << *this;

    std::istringstream iStr;
    boost::archive::text_iarchive ia (iStr);
    ia >> *this;
  }

  // ////////////////////////////////////////////////////////////////////
  template<class Archive>
  void OriginDestinationSetKey::serialize (Archive& ioArchive,
                                           const unsigned int iFileVersion) {
    /**
     * \note The serialised members should not be const (as, as far as
     *       I understand, they are tracked by Boost.Serialisation).
     */
    ioArchive & _destination;
  }

}
