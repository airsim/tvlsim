// STL
#include <iostream>
#include <string>
//
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
//
#include "networkFunctions.hpp"


int service_to_port (const std::string& s) {
	servent* a = getservbyname(s.c_str(), NULL);
	endservent();

	if (a!= NULL) {
		return htons(a -> s_port);
	} else {
		std::cout << "No service with name:" << s << " is found" << std::endl;
		return -1;
	}
}

std::string port_to_service (const int p){
	servent* a = getservbyport(htons(p), NULL);
	endservent();

	if (a!= NULL)
		return a -> s_name;
	else{
		std::cout << "No service with port:" << p << " is found" << std::endl;
		return "";
	}
}

std::string ip_to_hostname (const std::string& s){
	struct hostent *he;
	struct in_addr ipv4addr;
	inet_pton(AF_INET, s.c_str(), &ipv4addr);
	he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
	std::cout << "finish searching\n";
	if (he!= NULL)
		return he->h_name;
	else{
		std::cout << "Can't find hostname corresponding to: " << s << std::endl;
		return "";
	}
}

// int main(){
// 	std::cout << service_to_port("ariaa") << std::endl;
// 	std::cout << port_to_service(22) << std::endl;
// 	
// // 	string add="172.16.134.217";
// 	string add="173.163.134.217";
// 	std::cout << ip_to_hostname(add) << std::endl;
// 	return 0;
// }
