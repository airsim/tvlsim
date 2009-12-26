// Boost.ASIO Tutorial - Daytime4:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tutdaytime4.html
// STL
#include <iostream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// /////////// M A I N ////////////////
int main (int argc, char* argv[]) {

  boost::asio::io_service lIOService;

  std::cout << "We have waited 1 second" << std::endl;

  return 0;
}
