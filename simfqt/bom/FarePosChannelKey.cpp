// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ostream>
#include <sstream>
// STDAIR
#include <simfqt/bom/FarePosChannelKey.hpp>

namespace SIMFQT  {

  // ////////////////////////////////////////////////////////////////////
  FarePosChannelKey::FarePosChannelKey (const stdair::CityCode_T& iPosition,
                                        const stdair::ChannelLabel_T& iChannel)
    : _position (iPosition), _channel(iChannel) {
  }

  // ////////////////////////////////////////////////////////////////////
  FarePosChannelKey::FarePosChannelKey (const FarePosChannelKey& iKey)
    : _position (iKey._position), _channel (iKey._channel) {
  }

  // ////////////////////////////////////////////////////////////////////
  FarePosChannelKey::~FarePosChannelKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void FarePosChannelKey::toStream (std::ostream& ioOut) const {
    ioOut << "FarePosChannelKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void FarePosChannelKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string FarePosChannelKey::toString() const {
    std::ostringstream oStr;
    oStr << _position << "," << _channel << std::endl; 
    return oStr.str();
  }

}
