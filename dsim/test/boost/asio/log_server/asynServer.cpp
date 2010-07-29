// Boost.ASIO Tutorial - Daytime3:
// http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/tutorial/tutdaytime3.html
// STL
#include <cassert>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
// Boost.ASIO
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
// //////////////////////////////////////////////////////////
std::string make_daytime_string() {
  const std::time_t now = std::time(0);
  return std::ctime (&now);
}

// //////////////////////////////////////////////////////////
/** Class handling TCP connections for a given server. */
class TCPConnection :  public boost::enable_shared_from_this<TCPConnection> {
	public:
	/** Pointer on a TCP connection. */
	typedef boost::shared_ptr<TCPConnection> Pointer_T;

	/** Create a TCP connection, from a given Boost.ASIO service. */
	static Pointer_T create (boost::asio::io_service& ioIOService,  string lLogFile) {
		TCPConnection* oConnectionPtr = new TCPConnection (ioIOService, lLogFile);
		assert (oConnectionPtr != NULL);
		return Pointer_T (oConnectionPtr);
	}

	/** Get the underlying TCP socket. */
	boost::asio::ip::tcp::socket& socket() {
		return _socket;
	}

	/** Process the incoming client request, by giving it back the time of day. */
	void start() {
		ostringstream ostr;
		ostr << _socket.remote_endpoint();
		string clientFullName=ostr.str(); //127.0.0.1:73784
		string clientName= clientFullName.substr(0,clientFullName.find(":")); //127.0.0.1

		ostringstream oss;
		oss << clientName << ".log";
		log_file= oss.str(); // each client log into a seperate file which ends by log. Ex:127.0.0.1.log
		cout << "Received client: " << clientName << "           Corresponding log file:" << log_file << endl;
		
		ofstream out (log_file.c_str(), ios::app); // open log file
		if (out == NULL){
			cout << log_file <<  " :openning problem!" << endl;
			return ;
		}
		
		boost::system::error_code lTransferError;
		boost::array<char, 1024> lBuffer;
		for(;;){
			size_t lLength = _socket.read_some (boost::asio::buffer (lBuffer),  lTransferError);
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


	private:
	// //////////// Constructors & Destructors /////////////
	/** Constructor. */
	TCPConnection (boost::asio::io_service& ioIOService, string lLogFile)
		: _socket (ioIOService) {
			log_file = lLogFile;
	}

	void handleWrite (const boost::system::error_code& iErrorCode,
					const size_t iTransferredBytes) {
		// start();
	}


	private:
	// ///////////////// Technical Methods ///////////////////


	private:
	// /////////// Attributes /////////////
	/** Time of day. */
	std::string _message;

	/** TCP/IP socket. */
	boost::asio::ip::tcp::socket _socket;

	/** Log file prefix */
	string log_file;
};


/** Class starting a TCP server, and handling incoming requests. */
class TCPServer {
	public:
	// //////////// Constructors & Destructors /////////////
	/** Constructor.
		<br>Create a listener for IP/TCP v4 and listening on port lPort */
	TCPServer (boost::asio::io_service& ioIOService, int lPort, string lLogFile) : _acceptor (ioIOService,
					boost::asio::ip::tcp::endpoint (boost::asio::ip::tcp::v4(), lPort)) {
		log_file = lLogFile;
		startAccept();
		
	}


	private:
	// ///////////////// Technical Methods ///////////////////
	/** Accept (socket) connection from any client. */
	void startAccept() {
		TCPConnection::Pointer_T lConnection = TCPConnection::create (_acceptor.io_service(), log_file);

		boost::asio::ip::tcp::socket& lSocket = lConnection->socket();
		_acceptor.async_accept (lSocket,
								boost::bind (&TCPServer::handleAccept, this,
											lConnection,
											boost::asio::placeholders::error));
	}

	/** Process the (socket) connection from any client. */
	void handleAccept (TCPConnection::Pointer_T ioConnection,const boost::system::error_code& iError) {
		static int nbConnections = 0;
		nbConnections ++ ;
		cout << "Has received:"  << nbConnections << " clients" << endl;
		if (!iError) {
			ioConnection -> start();
			startAccept();
		}
	}


	private:
	// /////////// Attributes /////////////
	/** Connection acceptor. */
	boost::asio::ip::tcp::acceptor _acceptor;
	string log_file;
	};


// //////////////////// M A I N /////////////////////////////
int main (int argc, char* argv[]) {

	string log_file = "abcd.log";
	int listening_port = 2624;//corresponding to the "aria" service, as specified within the /etc/services file

	cout << "Syntax: ./asynServer  [file_to_log] [listening_port]" << endl;
	cout << "By default, server listens on port 2624 and file_to_log=\"abcd.log\"" << endl;

	if (argc > 2){
		listening_port = atoi(argv[2]);
		log_file = string(argv[1]);
	}
	cout << "=======================================================================================" << endl;
	cout << "Server log to file:" << log_file << endl;
	cout << "Server listening on port:" << listening_port << endl;
	cout << "=======================================================================================" << endl;

	
	try {

		boost::asio::io_service lIOService;
		TCPServer lServer (lIOService, listening_port, log_file);

		lIOService.run();

	} catch (std::exception& lException) {
		std::cerr << lException.what() << std::endl;
	}

	return 0;
}

