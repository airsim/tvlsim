#ifndef __INTRUSIVE_BOM_BOMABSTRACT_HPP
#define __INTRUSIVE_BOM_BOMABSTRACT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>

namespace intrusive {

  /** BomAbstract. */
  class BomAbstract {
  public:
    BomAbstract (const std::string& iKey) : _key (iKey)  {}
    BomAbstract (const int idx) {
      std::ostringstream oStr;
      oStr << idx;
      _key = oStr.str();
    }
  protected:
    std::string _key;
  };

}
#endif // __INTRUSIVE_BOM_BOMABSTRACT_HPP
