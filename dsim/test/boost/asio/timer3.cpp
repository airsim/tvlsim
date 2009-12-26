// Boost.ASIO Tutorial - Timer3:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tuttimer3.html
// STL
#include <iostream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// ////////////////////////////////////////////////////////
void print (const boost::system::error_code& iErrorCode,
            boost::asio::deadline_timer& ioTimerPtr, int& ioCount) {
  
  std::cout << "The call-back function has been triggered on a slave task";
  std::cout << ", after having waited for 1 second, for the "
            << ioCount << "-th time." << std::endl;
  
  if (ioCount <= 5) {
    ++ioCount;
    ioTimerPtr.expires_at (ioTimerPtr.expires_at()
                           + boost::posix_time::seconds(1));

    // See the note below about the Boost bind function
    ioTimerPtr.async_wait (boost::bind (print,
                                        boost::asio::placeholders::error,
                                        boost::ref (ioTimerPtr), ioCount));
  }
}

// /////////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

  boost::asio::io_service lIOService;
  int lCount = 1;
  boost::asio::deadline_timer lTimer (lIOService, boost::posix_time::seconds(1));

  // Asynchronous wait: when the timer reaches the deadline, the call-back
  // (here, the print() function) is called.
  //
  // Note: bind(print, error, timer, count) is equivalent to the
  //       print(error, timer, count) "nullary" function.
  // See http://www.boost.org/doc/libs/1_41_0/libs/bind/bind.html#Purpose
  // for more information on the Boost bind() function. For instance,
  // bind() copies the arguments, and as the Boost timer can not be copied,
  // a (Boost) reference must be explicitly taken.
  lTimer.async_wait (boost::bind (print,
                                  boost::asio::placeholders::error,
                                  boost::ref (lTimer), lCount));

  // Wait until the timer reaches the deadline. At that moment, the call-back
  // is called
  lIOService.run();

  std::cout << "The master task has come back in foreground." << std::endl;
  std::cout << "The timer has expired and been re-activated " << lCount
            << " times." << std::endl;

  return 0;
}
