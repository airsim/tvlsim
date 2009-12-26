// Boost.ASIO Tutorial - Timer2:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tuttimer2.html
// STL
#include <iostream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// ////////////////////////////////////////////////////////
void print (const boost::system::error_code& iErrorCode) {
  std::cout << "The call-back function has been triggered on a slave task";
  std::cout << ", after having waited for 1 second" << std::endl;
}

// /////////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

  boost::asio::io_service lIOService;
  boost::asio::deadline_timer lTimer (lIOService, boost::posix_time::seconds(1));

  // Asynchronous wait: when the timer reaches the deadline, the call-back
  // (here, the print() function) is called
  lTimer.async_wait (print);

  // Wait until the timer reaches the deadline. At that moment, the call-back
  // is called
  lIOService.run();

  std::cout << "The master task has come back in foreground" << std::endl;
  
  return 0;
}
