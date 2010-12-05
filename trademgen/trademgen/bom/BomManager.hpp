#ifndef __TRADEMGEN_BOM_BOMMANAGER_HPP
#define __TRADEMGEN_BOM_BOMMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>

namespace stdair {
  // Forward declarations
  struct BookingRequestStruct;
}

namespace TRADEMGEN {
  /** Utility class for Trademgen objects. */
  class BomManager {
  public:
    // //////////////// Display support methods /////////////////
    /** Recursively display (dump in the underlying output log stream)
        the objects of the given BOM tree.
        @param std::ostream& Output stream in which the BOM tree should be
               logged/dumped.
        @param const LegCabin& Root of the BOM tree to be displayed. */
    static void csvDisplay (std::ostream&,
                            const stdair::BookingRequestStruct&);
    /** Helper fuction to display an interger. */
    static void intDisplay (std::ostream&, const int&);
  };    

}
#endif // __TRADEMGEN_BOM_BOMMANAGER_HPP
