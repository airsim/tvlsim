
#ifndef __TVLSIM_NETWORKFUNCTIONS__
#define __TVLSIM_NETWORKFUNCTIONS__


/**
 * \brief Return the port corresponding to service name s using /etc/services
 * <br>For ex: aria returns 2624
 */
int service_to_port (const std::string&);


/**
 * \brief contrary of service_to_port
 * <br>ex: 22 -> ssh
 */
std::string port_to_service (const int p);

/**
 * \brief Ex: Takes 172.16.134.217 , returns nceorilnx03.nce.amadeus.net
 */
std::string ip_to_hostname (const std::string& s);


#endif // __TVLSIM_NETWORKFUNCTIONS__
