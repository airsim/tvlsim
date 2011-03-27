#ifndef __TRADEMGEN_BOM_BOMDISPLAY_HPP
#define __TRADEMGEN_BOM_BOMDISPLAY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// TraDemGen

/// Forward declarations
namespace stdair {
  class EventQueue;
}

namespace TRADEMGEN {

  /// Forward declarations
  class DemandStream;

  /**
   * @brief Utility class to display TraDemGen objects with a pretty
   * format.
   */
  class BomDisplay {
  public:
    // //////////////// Display support methods /////////////////
    /**
     * Recursively display (dump in the underlying output log stream)
     * the objects of the BOM tree.
     *
     * @param std::ostream& Output stream in which the BOM tree should be
     *        logged/dumped.
     * @param const stdair::EventQueue& Root of the BOM tree to be displayed.
     */
    static std::string csvDisplay (const stdair::EventQueue&);

    /**
     * Recursively display (dump in the underlying output log stream)
     * the objects of the BOM tree.
     *
     * @param std::ostream& Output stream in which the BOM tree should be
     *        logged/dumped.
     * @param const DemandStream& Root of the BOM tree to be displayed.
     */
    static void csvDisplay (std::ostream&, const DemandStream&);
  };
  
}
#endif // __TRADEMGEN_BOM_BOMDISPLAY_HPP
