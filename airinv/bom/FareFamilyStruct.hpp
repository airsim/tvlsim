#ifndef __AIRINV_BOM_FAREFAMILYSTRUCT_HPP
#define __AIRINV_BOM_FAREFAMILYSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// StdAir
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// AirInv
#include <airinv/bom/BookingClassStruct.hpp>

/// Forward declarations
namespace stdair {
  class FareFamily;
}

namespace AIRINV {

  /**
   * @brief Utility Structure for the parsing of fare family details.
   */
  struct FareFamilyStruct : public stdair::StructAbstract {
    // Attributes
    stdair::FamilyCode_T _familyCode;
    stdair::ClassList_String_T _classes;
    BookingClassStructList_T _classList;

    /**
     * Default constructor.
     */
    FareFamilyStruct();
    /**
     * Main constructor.
     */
    FareFamilyStruct (const stdair::FamilyCode_T&,
                      const stdair::ClassList_String_T&);
      
    /**
     * Fill the FareFamily objects with the attributes of the FareFamilyStruct.
     */
    void fill (stdair::FareFamily&) const;
      
    /**
     * Give a description of the structure (for display purposes).
     */
    const std::string describe() const;
  };

  /**
   * List of FareFamily-Detail structures.
   */
  typedef std::vector<FareFamilyStruct> FareFamilyStructList_T;

}
#endif // __AIRINV_BOM_FAREFAMILYSTRUCT_HPP
