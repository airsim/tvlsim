// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <string>
// Boost.ASIO
#include <boost/asio.hpp>
// Boost.Array
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

    boost::asio::ip::tcp::resolver::iterator itEndPoint =
      lResolver.resolve (lQuery);
    boost::asio::ip::tcp::resolver::iterator lEnd;

    boost::asio::ip::tcp::socket lSocket (lIOService);
    boost::system::error_code lError = boost::asio::error::host_not_found;

    //
    while (lError && itEndPoint != lEnd) {
      const boost::asio::ip::tcp::endpoint lEndPoint = *itEndPoint;

      // DEBUG
      std::cout << "Testing end point: " << std::endl;
      
      lSocket.close();
      lSocket.connect (lEndPoint, lError);
      ++itEndPoint;
    }

    //
    if (lError) {
      throw boost::system::system_error (lError);
    }
    assert (!lError);

    // DEBUG
    const boost::asio::ip::tcp::endpoint lValidEndPoint;
    std::cout << "Valid end point: " << lValidEndPoint << std::endl;

    // Send a message to the server
    const std::string lMessage ("Hello AirInv Server!");
    boost::asio::write (lSocket, boost::asio::buffer (lMessage),
                        boost::asio::transfer_all(), lError);
    
    // Read the reply from the server
    boost::array<char, 256> lBuffer;

    size_t lLength = lSocket.read_some (boost::asio::buffer(lBuffer), lError);

    // Some other error than connection closed cleanly by peer
    if (lError && lError != boost::asio::error::eof) {
      throw boost::system::system_error (lError);
    }
    
    // DEBUG
    std::cout << "Reply from the server: ";
    std::cout.write (lBuffer.data(), lLength);
    std::cout << std::endl;

  } catch (std::exception& lException) {
    std::cerr << lException.what() << std::endl;
  }

  return 0;
}
