// Log server
// STL
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <unistd.h>


// //////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {
	using namespace std;
	
	int listening_port = 2624;//corresponding to the "aria" service, as specified within the /etc/services file
	string log_file= "server.log";
	
	cout << "Syntax: ./server  [file_to_log] [listening_port]" << endl;
	cout << "By default, server listens on port 2624 and file_to_log=\"server.log\"" << endl;

	if (argc > 2){
		listening_port = atoi(argv[2]);
		log_file = string(argv[1]);
	}else if (argc == 2)
		log_file = string(argv[1]);
	
	cout << "=======================================================================================" << endl;
	cout << "Server log to file:" << log_file << endl;
	cout << "Server listening on port:" << listening_port << endl;
	cout << "=======================================================================================" << endl;


	try {

		boost::asio::io_service lIOService;

		// Create a listener for IP/TCP v4, listening on listening_port
		boost::asio::ip::tcp::acceptor lAcceptor (lIOService,
												boost::asio::ip::tcp::endpoint (boost::asio::ip::tcp::v4(), listening_port));

		int nbConnections = 0;												
		for (;;) {
			boost::asio::ip::tcp::socket lSocket (lIOService);
			lAcceptor.accept (lSocket); //will block until a new connection has been accepted successfully or an error occurs.
			nbConnections ++;
			std::cout << "Nb of connections:" << nbConnections << std::endl;
			cout << "received client: " << lSocket.remote_endpoint() << endl;

			boost::system::error_code lTransferError;
			boost::array<char, 1024> lBuffer;
			
			ofstream out (log_file.c_str(), ios::app); // open log file
			if (out == NULL){
				cout << log_file <<  " does not exist !" << endl;
				return -1;
			}
			for(;;){
				size_t lLength = lSocket.read_some (boost::asio::buffer (lBuffer),  lTransferError);
				out.write(lBuffer.data(),lLength);

				if (lTransferError == boost::asio::error::eof) {
					// Connection closed cleanly by peer.
					cout << "EOF found: Transfer finished\n >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n" ;
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

