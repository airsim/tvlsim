
#ifndef __NETWORKFUNCTIONS__
#define __NETWORKFUNCTIONS__


#include <iostream>
#include <string>

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* return the port corresponding to service name s using /etc/services
* For ex: aria returns 2624
*/
int service_to_port(std::string s);


/* contrary of service_to_port
* ex: 22 -> ssh
*/
std::string port_to_service(int p);

/* Ex:	Takes 172.16.134.217 , returns nceorilnx03.nce.amadeus.net*/
std::string ip_to_hostname(std::string s);


#endif