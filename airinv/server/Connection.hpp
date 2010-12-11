#ifndef __AIRINV_SVR_CONNECTION_HPP
#define __AIRINV_SVR_CONNECTION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
// Boost
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
// AirInv
#include <airinv/server/Reply.hpp>
#include <airinv/server/Request.hpp>

namespace AIRINV {

  // Forward declarations.
  class RequestHandler;
  

  /** Represents a single connection from a client. */
  class Connection : public boost::enable_shared_from_this<Connection>,
                     private boost::noncopyable {
  public:
    // //////////// Constructors and Destructors /////////////////
    /** Constructor.
        <br>Construct a connection with the given io_service. */
    Connection (boost::asio::io_service&, RequestHandler&);

    
    // /////////// Business Support Methods ////////////////
    /** Get the socket associated with the connection. */
    boost::asio::ip::tcp::socket& socket();

    /** Start the first asynchronous operation for the connection. */
    void start();

    
  private:
    /** Handle completion of a read operation. */
    void handleRead (const boost::system::error_code& e,
                      std::size_t bytes_transferred);

    /** Handle completion of a write operation. */
    void handleWrite (const boost::system::error_code& e);

    /** Strand to ensure the connection's handlers are not called
        concurrently. */
    boost::asio::io_service::strand _strand;

    /** Socket for the connection. */
    boost::asio::ip::tcp::socket _socket;

    /** The handler used to process the incoming request. */
    RequestHandler& _requestHandler;

    /** Buffer for incoming data. */
    boost::array<char, 8192> _buffer;

    /** The incoming request. */
    Request _request;

    /** The reply to be sent back to the client. */
    Reply _reply;
  };

  /** Shared pointer on a Connection object. */
  typedef boost::shared_ptr<Connection> ConnectionShrPtr_T;

}
#endif // __AIRINV_SVR_CONNECTION_HPP
