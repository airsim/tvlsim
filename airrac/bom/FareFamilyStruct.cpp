// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// AIRSCHED
#include <airsched/bom/FareFamilyStruct.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  FareFamilyStruct::
  FareFamilyStruct (const stdair::FamilyCode_T& iFamilyCode,
                      const stdair::ClassList_String_T& iClasses)
    : _familyCode (iFamilyCode),
      _classes (iClasses) {
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string FareFamilyStruct::describe() const {
    std::ostringstream ostr;
    ostr << "        " << _familyCode << " " << _classes << ", ";
    return ostr.str();
  }

}
