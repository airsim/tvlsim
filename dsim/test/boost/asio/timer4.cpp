// Boost.ASIO Tutorial - Timer4:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tuttimer4.html
// STL
#include <iostream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// ////////////////////////////////////////////////////////
class Printer {
public:
  /** Constructor. */
  Printer (boost::asio::io_service& lIOService)
    : _timer (lIOService, boost::posix_time::seconds(1)),
      _count (1) {

    // Asynchronous wait: when the timer reaches the deadline, the call-back
    // (here, the print() function) is called.
    //
    // Note: bind(print, error, timer, count) is equivalent to the
    //       print(error, timer, count) "nullary" function.
    // See http://www.boost.org/doc/libs/1_41_0/libs/bind/bind.html#Purpose
    // for more information on the Boost bind() function. For instance,
    // bind() copies the arguments, and as the Boost timer can not be copied,
    // a (Boost) reference must be explicitly taken.
    _timer.async_wait (boost::bind (&Printer::print, this,
                                    boost::asio::placeholders::error));
  }
  
  /** Destructor. */
  ~Printer() {
    std::cout << "The timer has expired and been re-activated " << _count
              << " times." << std::endl;
  }

  /** Call-back: display the counter. */  
  void print (const boost::system::error_code& iErrorCode) {
  
    std::cout << "The call-back function has been triggered on a slave task";
    std::cout << ", after having waited for 1 second, for the "
              << _count << "-th time." << std::endl;
  
    if (_count <= 5) {
      ++_count;
      _timer.expires_at (_timer.expires_at() + boost::posix_time::seconds(1));
      
      // See the note above about the Boost bind function
      _timer.async_wait (boost::bind (&Printer::print, this,
                                      boost::asio::placeholders::error));
    }
  }
  
  // //////// Attributes ////////
private:
  /** Timer. */
  boost::asio::deadline_timer _timer;
  /** Counter. */
  int _count;

};

// /////////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

  boost::asio::io_service lIOService;
  Printer lPrinter (lIOService);

  // Wait until the timer reaches the deadline. At that moment, the call-back
  // is called
  lIOService.run();

  std::cout << "The master task has come back in foreground." << std::endl;

  return 0;
}
