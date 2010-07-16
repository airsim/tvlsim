//Client
// STL
#include <iostream>
#include <string>
#include <fstream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/array.hpp>
// Sleep funtion
#include <unistd.h>

/* Send filename's content to server */
void send_file (std::string filename){
	using namespace std;
	std::string lHostname = "localhost";
	// Service name (as specified within /etc/services)
	// The "aria" service corresponds to the port 2624
	const std::string lServiceName = "aria";

	// try to get a socket (communication canal)
	try {
		boost::asio::io_service lIOService;
		boost::asio::ip::tcp::socket lSocket (lIOService);

		boost::asio::ip::tcp::resolver lResolver (lIOService);
		boost::asio::ip::tcp::resolver::query lQuery (lHostname, lServiceName);
		boost::asio::ip::tcp::resolver::iterator itEndPoint =lResolver.resolve (lQuery);
		boost::asio::ip::tcp::resolver::iterator lEnd;
		boost::system::error_code lError = boost::asio::error::host_not_found;

		while (lError && itEndPoint != lEnd) {
			const boost::asio::ip::tcp::endpoint lEndPoint = *itEndPoint;

			// DEBUG
// 			std::cout << "Testing end point: " << lEndPoint << std::endl;
// 			std::cout << "Testing lError: " << lError << std::endl;
			lSocket.close();
			lSocket.connect (lEndPoint, lError);
			++itEndPoint;
		}

		if (lError) {
			std::cout << "Cannot find corresponding endpoint" << std::endl;
			throw boost::system::system_error (lError);
		}
		assert (!lError);
		cout << "Socket is opened !\n";
		// File sending part
		fstream fst (filename.c_str(), ios::in);
		char * buffer; //contains file's content

		// get length of file:
		fst.seekg (0, ios::end);
		int length = fst.tellg();
		fst.seekg (0, ios::beg);

		cout << "file length:" << length << endl;
		buffer = new char[length];
		fst.read(buffer,length);

		boost::system::error_code lIgnoredError;
		boost::asio::write (lSocket, boost::asio::buffer (buffer),boost::asio::transfer_all(), lIgnoredError);
		
	} catch (std::exception& lException) {
		std::cerr << lException.what() << std::endl;
	}
}

// /////////// M A I N ////////////////
int main (int argc, char* argv[]) {
	using namespace std;
	string filename = "input";
	if (argc >= 2) {
		filename = string(argv[1]);
	}
	send_file(filename);
	return 0;
}


