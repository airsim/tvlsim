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
//Some network functions
#include "networkFunctions.cpp"


std::string lServerName = "localhost";
std::string lServiceName = "aria"; // The "aria" service corresponds to the port 2624 see /etc/services
std::string sendFile="test.txt";
int attempt=0; //Nb of attemps to connect to server
int lServerPort = 2624;

/* Send filename's content to server */
void send_file (std::string filename){
	using namespace std;
	// testing if file exists
	fstream fst (filename.c_str(), ios::in);
	if (fst == NULL){
		cout << filename <<  " does not exist !" << endl;
		exit(1);
	}
	
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

	// 			std::cout << "Testing end point: " << lEndPoint << std::endl;
	// 			std::cout << "Testing lError: " << lError << std::endl;
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

// /////////// M A I N ////////////////
int main (int argc, char* argv[]) {
	using namespace std;

	cout << "Syntax: ./client [file_to_send] [server_name] [server_port] " << endl;
	cout << "By default, file_to_send= \"test.txt\"  and server_name=\"localhost\" and server_port=2624	"  << endl;
	
	if (argc > 3) {
		sendFile = string(argv[1]);
		lServerName = string (argv[2]);
		lServerPort = atoi(argv[3]);
		lServiceName = port_to_service(lServerPort);
	}else if (argc == 2)
		sendFile = string(argv[1]);

	cout << "=======================================================================================" << endl;
	cout << "File to send:" << sendFile << endl;
	cout << "Server name:" << lServerName << endl;
	cout << "Server port:" << lServerPort << endl;
	cout << "=======================================================================================" << endl;
	
	send_file(sendFile);
	return 0;
}


