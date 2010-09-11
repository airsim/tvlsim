#ifndef __INTRUSIVE_BOM_LEGDATE_HPP
#define __INTRUSIVE_BOM_LEGDATE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// 
#include <test/boost/intrusive/BomAbstract.hpp>

/** Alias for the boost::intrusive namespace. */
namespace bi = boost::intrusive;

namespace intrusive {

  /** LegDate. */
  class LegDate : public BomAbstract {
  public:
    LegDate (const std::string& iKey) : BomAbstract (iKey)  {}
    LegDate (const int idx) : BomAbstract (idx) {}
    
    bi::list_member_hook<> _childHook;
    bi::list_member_hook<> _siblingHook;
  };

  /** List of child-type LegDate objects. */
  typedef bi::member_hook <LegDate, bi::list_member_hook<>,
                           &LegDate::_childHook> LegDateChildMemberOption;
  typedef bi::list<LegDate, LegDateChildMemberOption> LegDateChildren;

  /** List of sibling-type LegDate objects. */
  typedef bi::member_hook <LegDate, bi::list_member_hook<>,
                           &LegDate::_siblingHook> LegDateSiblingMemberOption;
  typedef bi::list<LegDate, LegDateSiblingMemberOption> LegDateSiblings;

}
#endif // __INTRUSIVE_BOM_LEGDATE_HPP
