// Boost.ASIO Tutorial - Timer1:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tuttimer1.html
// STL
#include <iostream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// /////////// M A I N ////////////////
int main (int argc, char* argv[]) {

  boost::asio::io_service lIOService;
  boost::asio::deadline_timer lTimer (lIOService, boost::posix_time::seconds(1));

  lTimer.wait();

  std::cout << "We have waited 1 second" << std::endl;

  return 0;
}
