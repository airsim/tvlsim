#ifndef __TVLSIM_TINYLOG__
#define __TVLSIM_TINYLOG__

// String, stream functions
#include <string>

/**
 * \brief Send filename's content to server
 * \param[in] lServerName: name of server,ex: localhost
 * \param[in] lServerPort: listening port on server
 * \param[in] filename: file to send to server
*/
void send_file (const std::string& lServerName, const int lServerPort,
                const std::string& filename);

/**
 * \brief Send string buffer to server
 * \param[in] lServerName: name of server,ex: localhost
 * \param[in] lServerPort: listening port on server
 * \param[in] filename: file to send to server
*/
void send_string (const std::string& lServerName, const int lServerPort,
                  const std::string& buffer);


#endif // __TVLSIM_TINYLOG__
