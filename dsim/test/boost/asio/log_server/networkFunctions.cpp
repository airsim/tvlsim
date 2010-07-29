// STL
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

/* return the port corresponding to service name s using /etc/services
For ex: aria returns 2624
*/
int service_to_port(string s){
	servent* a = getservbyname(s.c_str(), NULL);
	endservent();

	if (a!= NULL)
		return htons(a -> s_port);
	else{
		cout << "No service with name:" << s << " is found" << endl;
		return -1;
	}
}
string port_to_service(int p){
	servent* a = getservbyport(htons(p), NULL);
	endservent();

	if (a!= NULL)
		return a -> s_name;
	else{
		cout << "No service with port:" << p << " is found" << endl;
		return "";
	}
}
/* Ex:	Takes 172.16.134.217 , returns nceorilnx03.nce.amadeus.net*/
string ip_to_hostname(string s){
	struct hostent *he;
	struct in_addr ipv4addr;
	inet_pton(AF_INET, s.c_str(), &ipv4addr);
	he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
	cout << "finish searching\n";
	if (he!= NULL)
		return he->h_name;
	else{
		cout << "Can't find hostname corresponding to: " << s << endl;
		return "";
	}
		
	


}
// int main(){
// 	cout << service_to_port("ariaa") << endl;
// 	cout << port_to_service(22) << endl;
// 	
// // 	string add="172.16.134.217";
// 	string add="173.163.134.217";
// 	cout << ip_to_hostname(add) << endl;
// 	return 0;
// }