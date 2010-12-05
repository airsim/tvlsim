// Boost.ASIO Tutorial - Timer5:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tuttimer5.html
// STL
#include <iostream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// ////////////////////////////////////////////////////////
class Printer {
public:
  /** Constructor. */
  Printer (boost::asio::io_service& lIOService)
    : _strand (lIOService),
      _timer1 (lIOService, boost::posix_time::seconds(1)),
      _timer2 (lIOService, boost::posix_time::seconds(1)),
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
    _timer1.async_wait (_strand.wrap (boost::bind (&Printer::print1, this,
                                                   boost::asio::placeholders::error)));
    _timer2.async_wait (_strand.wrap (boost::bind (&Printer::print2, this,
                                                   boost::asio::placeholders::error)));
  }
  
  /** Destructor. */
  ~Printer() {
    std::cout << "The timer has expired and been re-activated " << _count
              << " times." << std::endl;
  }

  /** Call-back #1: display the shared counter. */  
  void print1 (const boost::system::error_code& iErrorCode) {
  
    std::cout << "The call-back function has been triggered on slave task #1";
    std::cout << ", after having waited for 1 second, for the "
              << _count << "-th time." << std::endl;
  
    if (_count <= 5) {
      ++_count;
      _timer1.expires_at (_timer1.expires_at() + boost::posix_time::seconds(1));
      
      // See the note above about the Boost bind function
      _timer1.async_wait (_strand.wrap (boost::bind (&Printer::print1, this,
                                                     boost::asio::placeholders::error)));
    }
  }
  
  /** Call-back #2: display the shared counter. */  
  void print2 (const boost::system::error_code& iErrorCode) {
  
    std::cout << "The call-back function has been triggered on slave task #2";
    std::cout << ", after having waited for 1 second, for the "
              << _count << "-th time." << std::endl;
  
    if (_count <= 5) {
      ++_count;
      _timer2.expires_at (_timer2.expires_at() + boost::posix_time::seconds(1));
      
      // See the note above about the Boost bind function
      _timer2.async_wait (_strand.wrap (boost::bind (&Printer::print2, this,
                                                     boost::asio::placeholders::error)));
    }
  }
  
  // //////// Attributes ////////
private:
  /** Strand (to synchronise the call of call-backs). */
  boost::asio::strand _strand;
  /** Timer2. */
  boost::asio::deadline_timer _timer1;
  boost::asio::deadline_timer _timer2;
  /** Shared counter. */
  int _count;
};

// /////////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

  boost::asio::io_service lIOService;
  Printer lPrinter (lIOService);

  // Spawn a new thread, itself invoking the lIOService::run() function
  boost::thread lThread (boost::bind (&boost::asio::io_service::run,
                                      &lIOService));
  
  // Wait until the timer reaches the deadline. At that moment, the call-back
  // is called
  lIOService.run();

  // Wait for the thread to return
  lThread.join();

  std::cout << "The master task has come back in foreground." << std::endl;

  return 0;
}
