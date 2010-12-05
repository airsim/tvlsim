// Boost.ASIO Tutorial - Daytime2:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tutdaytime2.html
// STL
#include <iostream>
#include <string>
#include <ctime>
// Boost.ASIO
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// //////////////////////////////////////////////////////////
std::string make_daytime_string() {
  const std::time_t now = std::time(0);
  return std::ctime (&now);
}

// //////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

  try {
    
    boost::asio::io_service lIOService;

    // Create a listener for IP/TCP v4, listening on port 2624 (corresponding
    // to the "aria" service, as specified within the /etc/services file)
    boost::asio::ip::tcp::acceptor lAcceptor (lIOService,
                                              boost::asio::ip::tcp::endpoint (boost::asio::ip::tcp::v4(), 2624));

    for (;;) {
      boost::asio::ip::tcp::socket lSocket (lIOService);
      lAcceptor.accept (lSocket);
      const std::string lMessage = make_daytime_string();
      
      boost::system::error_code lIgnoredError;
      boost::asio::write (lSocket, boost::asio::buffer (lMessage),
                          boost::asio::transfer_all(), lIgnoredError);
    }
    
  } catch (std::exception& lException) {
    std::cerr << lException.what() << std::endl;
  }

  return 0;
}

