#ifndef __TINYLOG__
#define __TINYLOG__

// String, stream functions
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

/* Send filename's content to server
* lServerName: name of server,ex: localhost
* lServerPort: listening port on server
* filename: file to send to server
*/
void send_file (std::string lServerName, int lServerPort, std::string filename);

/* Send string buffer to server
* lServerName: name of server,ex: localhost
* lServerPort: listening port on server
* filename: file to send to server
*/
void send_string(std::string lServerName, int lServerPort, std::string buffer);


#endif