// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// Boost
#include <boost/lexical_cast.hpp>
// AirInv
#include <airinv/server/Reply.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  std::vector<boost::asio::const_buffer> Reply::to_buffers() {
    std::vector<boost::asio::const_buffer> lBuffers;
    lBuffers.push_back (boost::asio::buffer(content));
    return lBuffers;
  }

}
