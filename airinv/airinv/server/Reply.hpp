#ifndef __AIRINV_SVR_REPLY_HPP
#define __AIRINV_SVR_REPLY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// Boost
#include <boost/asio.hpp>
// AirInv
#include <airinv/FlightRequestStatus.hpp>

namespace AIRINV {
  
  /** A reply to be sent to a client. */
  struct Reply {
    /** Status. */
    FlightRequestStatus::EN_FlightRequestStatus _status;
    
    /** The content to be sent in the reply. */
    std::string content;
    
    /** Convert the reply into a vector of buffers. The buffers do
        not own the underlying memory blocks, therefore the reply
        object must remain valid and not be changed until the write
        operation has completed. */
    std::vector<boost::asio::const_buffer> to_buffers();
  };

}
#endif // __AIRINV_SVR_REPLY_HPP
