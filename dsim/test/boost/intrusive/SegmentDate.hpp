#ifndef __INTRUSIVE_BOM_SEGMENTDATE_HPP
#define __INTRUSIVE_BOM_SEGMENTDATE_HPP

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

  /** SegmentDate. */
  class SegmentDate : public BomAbstract {
  public:
    SegmentDate (const std::string& iKey) : BomAbstract (iKey)  {}
    SegmentDate (const int idx) : BomAbstract (idx) {}
    
    bi::list_member_hook<> _childHook;
    bi::list_member_hook<> _siblingHook;
  };

  /** List of child-type SegmentDate objects. */
  typedef bi::member_hook <SegmentDate, bi::list_member_hook<>,
                           &SegmentDate::_childHook> SegmentDateChildMemberOption;
  typedef bi::list<SegmentDate,
                   SegmentDateChildMemberOption> SegmentDateChildren;

  /** List of sibling-type SegmentDate objects. */
  typedef bi::member_hook <SegmentDate, bi::list_member_hook<>,
                           &SegmentDate::_siblingHook> SegmentDateSiblingMemberOption;
  typedef bi::list<SegmentDate,
                   SegmentDateSiblingMemberOption> SegmentDateSiblings;

}
#endif // __INTRUSIVE_BOM_SEGMENTDATE_HPP
