// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/FareFamily.hpp>
// AirInv
#include <airinv/bom/FareFamilyStruct.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  FareFamilyStruct::FareFamilyStruct()
    : _familyCode (stdair::DEFAULT_NULL_FARE_FAMILY_CODE),
      _classes (stdair::DEFAULT_NULL_CLASS_CODE) {  
  }

  // //////////////////////////////////////////////////////////////////////
  FareFamilyStruct::
  FareFamilyStruct (const stdair::FamilyCode_T& iFamilyCode,
                    const stdair::ClassList_String_T& iClasses)
    : _familyCode (iFamilyCode), _classes (iClasses) {
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string FareFamilyStruct::describe() const {
    std::ostringstream ostr;

    ostr << "        " << _familyCode << " " << _classes << ", ";

    for (BookingClassStructList_T::const_iterator itBkgClass= _classList.begin();
         itBkgClass != _classList.end(); ++itBkgClass) {
      const BookingClassStruct& lBkgClass = *itBkgClass;
      ostr << lBkgClass.describe();
    }
    if (_classList.empty() == false) {
      ostr << std::endl;
    }

    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void FareFamilyStruct::fill (stdair::FareFamily& ioFareFamily) const {
    // Set attributes
    // ioFareFamily.setSomeAttribute (_someAttribute);
  }

}
