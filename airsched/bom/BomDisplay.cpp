// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// StdAir
#include <stdair/basic/BasConst_BomDisplay.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
// AirSched
#include <airsched/bom/ReachableUniverse.hpp>
#include <airsched/bom/BomDisplay.hpp>

namespace AIRSCHED {

  /**
   * Helper singleton structure to store the current formatting flags
   * of any given output stream. The flags are re-set at the
   * structure destruction.
   */
  struct FlagSaver {
  public:
    /** Constructor. */
    FlagSaver (std::ostream& oStream)
      : _oStream (oStream), _streamFlags (oStream.flags()) {
    }

    /** Destructor. */
    ~FlagSaver() {
      // Reset formatting flags of the given output stream
      _oStream.flags (_streamFlags);
    }
    
  private:
    /** Reference on the STL stream, for which the flags must be saved. */
    std::ostream& _oStream;
    /** Saved STL stream flags. */
    std::ios::fmtflags _streamFlags;
  };

  // ////////////////////////////////////////////////////////////////////
  std::string BomDisplay::csvDisplay (const stdair::BomRoot& iBomRoot) {
    std::ostringstream oStream;

    /**
     * Bom root level (only)
     */
    oStream << std::endl;
    oStream << "==============================================================="
            << std::endl;
    oStream << "BomRoot: " << iBomRoot.describeKey() << std::endl;
    oStream << "==============================================================="
            << std::endl;

    // Check whether there are ReachableUniverse objects
    if (stdair::BomManager::hasList<ReachableUniverse> (iBomRoot) == false) {
      return oStream.str();
    }
    
    // Retrieve the ReachableUniverse list
    const ReachableUniverseList_T& lReachableUniverseList =
      stdair::BomManager::getList<ReachableUniverse> (iBomRoot);

    // Browse the networks for each departure airport
    for (ReachableUniverseList_T::const_iterator itReachableUniverse =
           lReachableUniverseList.begin();
         itReachableUniverse != lReachableUniverseList.end();
         ++itReachableUniverse) {
      ReachableUniverse* lReachableUniverse_ptr = *itReachableUniverse;
      assert (lReachableUniverse_ptr != NULL);

      // Display the reachable universe
      csvDisplay (oStream, *lReachableUniverse_ptr);
    }

    return oStream.str();
  }

  // ////////////////////////////////////////////////////////////////////
  void BomDisplay::csvDisplay (std::ostream& oStream,
                               const ReachableUniverse& iReachableUniverse) {
    // Save the formatting flags for the given STL output stream
    FlagSaver flagSaver (oStream);

    /**
     * ReachableUniverse level (only)
     */
    oStream << "+++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    oStream << iReachableUniverse.toString();
    oStream << "+++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  }

}
