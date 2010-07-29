
// String, stream functions
#include <string>
#include <fstream>
// Boost.ASIO
#include <boost/asio.hpp>
#include <boost/array.hpp>
// Sleep funtion
#include <unistd.h>
//Some network functions
#include "networkFunctions.cpp"
//
#include "tinyLog.hpp"

void send_file (const std::string& lServerName, const int lServerPort,
                const std::string& filename) {
	using namespace std;
	int attempt=0; //Nb of attemps to connect to server
	// testing if file exists
	fstream fst (filename.c_str(), ios::in);
	if (fst == NULL){
		cout << filename <<  " does not exist !" << endl;
		exit(1);
	}
	std::string lServiceName= port_to_service(lServerPort);
	// try to get a socket (communication canal)
	while(1){

		boost::asio::io_service lIOService;
		boost::asio::ip::tcp::socket lSocket (lIOService);
		boost::asio::ip::tcp::resolver lResolver (lIOService);
		boost::asio::ip::tcp::resolver::query lQuery (lServerName, lServiceName);

		boost::asio::ip::tcp::resolver::iterator lEnd;
		boost::system::error_code lError = boost::asio::error::host_not_found;
		try {
			boost::asio::ip::tcp::resolver::iterator itEndPoint =lResolver.resolve (lQuery);

			while (lError && itEndPoint != lEnd) {
				const boost::asio::ip::tcp::endpoint lEndPoint = *itEndPoint;

				lSocket.close();
				lSocket.connect (lEndPoint, lError);
				++itEndPoint;
			}

			if (lError) {
				std::cout << "Cannot find corresponding endpoint at:" << attempt << " attempts" << std::endl;
				attempt ++;
				sleep(2); // not good to be too active
				continue;
				throw boost::system::system_error (lError);
			}
			assert (!lError);
// 			cout << "Socket is opened !\n";

			// File sending part
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

			cout << "Transfer finished" << endl;
			return;
		} catch (std::exception& lException) {
			std::cerr << lException.what() << std::endl;
		}
	}
}

void send_string (const std::string& lServerName, const int lServerPort,
                  const std::string& buffer) {
	using namespace std;
	int attempt=0; //Nb of attemps to connect to server

	std::string lServiceName= port_to_service(lServerPort);
	// try to get a socket (communication canal)
	while(1){

		boost::asio::io_service lIOService;
		boost::asio::ip::tcp::socket lSocket (lIOService);
		boost::asio::ip::tcp::resolver lResolver (lIOService);
		boost::asio::ip::tcp::resolver::query lQuery (lServerName, lServiceName);

		boost::asio::ip::tcp::resolver::iterator lEnd;
		boost::system::error_code lError = boost::asio::error::host_not_found;
		try {
			boost::asio::ip::tcp::resolver::iterator itEndPoint =lResolver.resolve (lQuery);

			while (lError && itEndPoint != lEnd) {
				const boost::asio::ip::tcp::endpoint lEndPoint = *itEndPoint;

				lSocket.close();
				lSocket.connect (lEndPoint, lError);
				++itEndPoint;
			}

			if (lError) {
				std::cout << "Cannot find corresponding endpoint at:" << attempt << " attempts" << std::endl;
				attempt ++;
				sleep(2); // not good to be too active
				continue;
				throw boost::system::system_error (lError);
			}
			assert (!lError);
// 			cout << "Socket is opened !\n";		

			boost::system::error_code lIgnoredError;
			boost::asio::write (lSocket, boost::asio::buffer (buffer),boost::asio::transfer_all(), lIgnoredError);

			cout << "Transfer finished" << endl;
			return;
		} catch (std::exception& lException) {
			std::cerr << lException.what() << std::endl;
		}
	}
}



