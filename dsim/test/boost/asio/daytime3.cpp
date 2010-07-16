// Boost.ASIO Tutorial - Daytime3:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tutdaytime3.html
// STL
#include <cassert>
#include <ctime>
#include <iostream>
#include <string>
// Boost.ASIO
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// //////////////////////////////////////////////////////////
std::string make_daytime_string() {
  const std::time_t now = std::time(0);
  return std::ctime (&now);
}

// //////////////////////////////////////////////////////////
/** Class handling TCP connections for a given server. */
class TCPConnection :
  public boost::enable_shared_from_this<TCPConnection> {
public:
  /** Pointer on a TCP connection. */
  typedef boost::shared_ptr<TCPConnection> Pointer_T;

  /** Create a TCP connection, from a given Boost.ASIO service. */
  static Pointer_T create (boost::asio::io_service& ioIOService) {
    TCPConnection* oConnectionPtr = new TCPConnection (ioIOService);
    assert (oConnectionPtr != NULL);
    return Pointer_T (oConnectionPtr);
  }

  /** Get the underlying TCP socket. */
  boost::asio::ip::tcp::socket& socket() {
    return _socket;
  }

  /** Process the incoming client request, by giving it back the time of day. */
  void start() {
    
    _message = make_daytime_string();
      
    boost::system::error_code lIgnoredError;
    boost::asio::async_write (_socket, boost::asio::buffer (_message),
                              boost::bind (&TCPConnection::handleWrite,
                                           shared_from_this(),
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
  }


private:
  // //////////// Constructors & Destructors /////////////
  /** Constructor. */
  TCPConnection (boost::asio::io_service& ioIOService)
    : _socket (ioIOService) {
  }

 void handleWrite (const boost::system::error_code& iErrorCode,
                   const size_t iTransferredBytes) {
	// start();
  }
 
  
private:
  // ///////////////// Technical Methods ///////////////////
  

private:
  // /////////// Attributes /////////////
  /** Time of day. */
  std::string _message;

  /** TCP/IP socket. */
  boost::asio::ip::tcp::socket _socket;
};


/** Class starting a TCP server, and handling incoming requests. */
class TCPServer {
public:
  // //////////// Constructors & Destructors /////////////
  /** Constructor.
      <br>Create a listener for IP/TCP v4, listening on port 2624 (corresponding
      to the "aria" service, as specified within the /etc/services file) */
  TCPServer (boost::asio::io_service& ioIOService)
    : _acceptor (ioIOService,
                 boost::asio::ip::tcp::endpoint (boost::asio::ip::tcp::v4(),
                                                 2624)) {
    startAccept();
  }

  
private:
  // ///////////////// Technical Methods ///////////////////
  /** Accept (socket) connection from any client. */
  void startAccept() {
    TCPConnection::Pointer_T lConnection =
      TCPConnection::create (_acceptor.io_service());

    boost::asio::ip::tcp::socket& lSocket = lConnection->socket();
    _acceptor.async_accept (lSocket,
                            boost::bind (&TCPServer::handleAccept, this,
                                         lConnection,
                                         boost::asio::placeholders::error));
  }

  /** Process the (socket) connection from any client. */
  void handleAccept (TCPConnection::Pointer_T ioConnection,
                     const boost::system::error_code& iError) {

    if (!iError) {
      ioConnection->start();
      startAccept();
    }
  }

  
private:
  // /////////// Attributes /////////////
  /** Connection acceptor. */
  boost::asio::ip::tcp::acceptor _acceptor;
};


// //////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

  try {
    
    boost::asio::io_service lIOService;
    TCPServer lServer (lIOService);

    lIOService.run();
   
  } catch (std::exception& lException) {
    std::cerr << lException.what() << std::endl;
  }

  return 0;
}

