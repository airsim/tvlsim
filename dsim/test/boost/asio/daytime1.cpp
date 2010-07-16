// Boost.ASIO Tutorial - Daytime1:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tutdaytime1.html
// STL
#include <iostream>
#include <string>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/array.hpp>

// /////////// M A I N ////////////////
int main (int argc, char* argv[]) {

  // Host name
  std::string lHostname = "localhost";

  // Service name (as specified within /etc/services)
  // The "aria" service corresponds to the port 2624
  const std::string lServiceName = "aria";
  
  try {

    if (argc >= 2) {
      lHostname = argv[1];
    }

    boost::asio::io_service lIOService;

    boost::asio::ip::tcp::resolver lResolver (lIOService);

    boost::asio::ip::tcp::resolver::query lQuery (lHostname, lServiceName);
//     boost::asio::ip::tcp::resolver::query lQuery (lHostname, "daytime");

    boost::asio::ip::tcp::resolver::iterator itEndPoint =
      lResolver.resolve (lQuery);
    boost::asio::ip::tcp::resolver::iterator lEnd;

    boost::asio::ip::tcp::socket lSocket (lIOService);
    boost::system::error_code lError = boost::asio::error::host_not_found;
    
    while (lError && itEndPoint != lEnd) {
      const boost::asio::ip::tcp::endpoint lEndPoint = *itEndPoint;

      // DEBUG
      std::cout << "Testing end point: " << std::endl;
      
      lSocket.close();
      lSocket.connect (lEndPoint, lError);
      ++itEndPoint;
    }
    
    if (lError) {
      throw boost::system::system_error (lError);
    }
    assert (!lError);

    // DEBUG
    const boost::asio::ip::tcp::endpoint lValidEndPoint;
    std::cout << "Valid end point: " << lValidEndPoint << std::endl;

    for (;;) {
      boost::array<char, 128> lBuffer;
      boost::system::error_code lError;

      size_t lLength = lSocket.read_some (boost::asio::buffer (lBuffer), lError);

      if (lError == boost::asio::error::eof) {
        // Connection closed cleanly by peer.
        break;
        
      } else if (lError) {
        // Some other error.
        throw boost::system::system_error (lError);
      }
      
      std::cout.write (lBuffer.data(), lLength);
    }

  } catch (std::exception& lException) {
    std::cerr << lException.what() << std::endl;
  }

  return 0;
}
