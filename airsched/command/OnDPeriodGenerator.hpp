#ifndef __AIRSCHED_CMD_ONDPERIODGENERATOR_HPP
#define __AIRSCHED_CMD_ONDPERIODGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// AirSched
#include <airsched/AIRSCHED_Types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRSCHED {

  // Forward declarations
  struct OnDPeriodStruct_T;
  namespace OnDParserHelper {
    struct doEndOnD;
  }
    
  /** Class handling the generation / instantiation of the O&D-Period BOM. */
  class OnDPeriodGenerator : public stdair::CmdAbstract {
    // Only the following class may use methods of OnDPeriodGenerator.
    // Indeed, as those methods build the BOM, it is not good to expose
    // them publicly.
    friend class OnDPeriodFileParser;
    friend struct OnDParserHelper::doEndOnD;
    friend class OnDParser;

  private:
    /** Generate the O&D-period objects corresponding to the given
        OnDPeriodStruct, and add them to the given BomRoot. */
    static void createOnDPeriod (stdair::BomRoot&,
                                 const OnDPeriodStruct&);

  };

}
#endif // __AIRSCHED_CMD_ONDPERIODGENERATOR_HPP
