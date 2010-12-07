#ifndef __AIRINV_SVR_HEADER_HPP
#define __AIRINV_SVR_HEADER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace AIRINV {

  /** Header structure. */
  struct header {
    std::string name;
    std::string value;
  };

}
#endif // __AIRINV_SVR_HEADER_HPP
