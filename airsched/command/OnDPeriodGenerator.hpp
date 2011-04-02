#ifndef __AIRSCHED_CMD_ONDPERIODGENERATOR_HPP
#define __AIRSCHED_CMD_ONDPERIODGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// AirSched
#include <airsched/AIRSCHED_Types.hpp>

/// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRSCHED {

  /// Forward declarations
  struct OnDPeriodStruct_T;
  namespace OnDParserHelper {
    struct doEndOnD;
  }
    
  /**
   * @brief Class handling the generation / instantiation of the
   *        O&D-Period BOM.
   */
  class OnDPeriodGenerator : public stdair::CmdAbstract {
    /**
     * Only the following class may use methods of OnDPeriodGenerator.
     * Indeed, as those methods build the BOM, it is not good to expose
     * them publicly.
     */
    friend class OnDPeriodFileParser;
    friend struct OnDParserHelper::doEndOnD;
    friend class OnDParser;

  private:
    /**
     * Generate the O&D-period object corresponding to the given
     * OnDPeriodStruct, and add it to the given BomRoot.
     *
     * @param stdair::BomRoot& BOM tree root.
     * @param const OnDPeriodStruct& Structure holding the content to
     *        be copied into the to-be-created BOM object.
     */
    static void createOnDPeriod (stdair::BomRoot&, const OnDPeriodStruct&);
  };

}
#endif // __AIRSCHED_CMD_ONDPERIODGENERATOR_HPP
