#ifndef __AIRINV_SVR_AIRINVSERVER_HPP
#define __AIRINV_SVR_AIRINVSERVER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// Boost
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
// AirInv
#include <airinv/server/Connection.hpp>
#include <airinv/server/RequestHandler.hpp>

namespace AIRINV {

  /** The top-level class of the AirInv server. */
  class AirInvServer : private boost::noncopyable {
  public:
    // //////////// Constructors and Destructors /////////////////
    /** Constructor.
        <br>Construct the server to listen on the specified TCP address
        and port, and serve up files from the given directory. */
    AirInvServer (const std::string& address, const std::string& port,
                  const stdair::AirlineCode_T& iAirlineCode,
                  std::size_t thread_pool_size);
    /** Destructor. */
    ~AirInvServer();


  public:
    // ////////////////// Business Methods ////////////////////
    /** Run the server's io_service loop. */
    void run();

    /** Stop the server. */
    void stop();

    
  private:
    // //////////// Constructors and Destructors /////////////////
    /** Default constructors. */
    AirInvServer();
    AirInvServer(const AirInvServer&);

    
  private:
    // //////////// Attributes /////////////////
    /** Handle completion of an asynchronous accept operation. */
    void handleAccept (const boost::system::error_code& e);

    /** The number of threads that will call io_service::run(). */
    std::size_t _threadPoolSize;

    /** The boost::asio::io_service used to perform asynchronous operations. */
    boost::asio::io_service _ioService;

    /** Acceptor used to listen for incoming connections. */
    boost::asio::ip::tcp::acceptor _acceptor;

    /** The next connection to be accepted. */
    ConnectionShrPtr_T _newConnection;

    /** The handler for all incoming requests. */
    RequestHandler _requestHandler;
  };

}
#endif // __AIRINV_SVR_AIRINVSERVER_HPP
