// Log server
// STL
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;

// //////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {
	using namespace std;
	try {

		boost::asio::io_service lIOService;

		// Create a listener for IP/TCP v4, listening on port 2624 (corresponding
		// to the "aria" service, as specified within the /etc/services file)
		boost::asio::ip::tcp::acceptor lAcceptor (lIOService,
												boost::asio::ip::tcp::endpoint (boost::asio::ip::tcp::v4(), 2624));

		int nbConnections = 0;												
		for (;;) {
			boost::asio::ip::tcp::socket lSocket (lIOService);
			lAcceptor.accept (lSocket);
			nbConnections ++;
			std::cout << "Nb of connections:" << nbConnections << std::endl;
		
			boost::system::error_code lIgnoredError;
			boost::system::error_code lTransferError;
			boost::array<char, 1024> lBuffer;
			
			ofstream out ("server.log", ios::app); //file to write log
			
			for(;;){
				size_t lLength = lSocket.read_some (boost::asio::buffer (lBuffer),  lTransferError);
				out.write(lBuffer.data(),lLength);
				
				if (lTransferError == boost::asio::error::eof) {
					// Connection closed cleanly by peer.
					cout << "EOF error\n" ;
					break;
				} else if (lTransferError) {
					// Some other error.
					throw boost::system::system_error (lTransferError);
				}
// 				std::cout.write (lBuffer.data(), lLength);
			}
			out.close();
		}

	} catch (std::exception& lException) {
		std::cerr << lException.what() << std::endl;
	}

	return 0;
}

