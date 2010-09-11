#ifndef __INTRUSIVE_BOM_BOMROOT_HPP
#define __INTRUSIVE_BOM_BOMROOT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// 
#include <test/boost/intrusive/BomAbstract.hpp>

namespace intrusive {

  /** BomRoot. */
  class BomRoot : public BomAbstract {
  public:
    BomRoot (const std::string& iKey) : BomAbstract (iKey)  {}
    BomRoot (const int idx) : BomAbstract (idx) {}
  };

}
#endif // __INTRUSIVE_BOM_BOMROOT_HPP
