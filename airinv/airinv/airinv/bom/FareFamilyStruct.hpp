#ifndef __AIRINV_BOM_FAREFAMILYSTRUCT_HPP
#define __AIRINV_BOM_FAREFAMILYSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// STDAIR
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>

namespace AIRINV {

  /** Utility Structure for the parsing of fare family details. */
  struct FareFamilyStruct : public stdair::StructAbstract {
    // Attributes
    stdair::FamilyCode_T _familyCode;
    stdair::ClassList_String_T _classes;

    /** Constructors. */
    FareFamilyStruct (const stdair::FamilyCode_T&,
                        const stdair::ClassList_String_T&);
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;
  };

  /** List of FareFamily-Detail structures. */
  typedef std::vector<FareFamilyStruct> FareFamilyStructList_T;

}
#endif // __AIRINV_BOM_FAREFAMILYSTRUCT_HPP