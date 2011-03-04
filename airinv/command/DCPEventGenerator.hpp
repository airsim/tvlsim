#ifndef __AIRINV_CMD_DCPEVENTGENERATOR_HPP
#define __AIRINV_CMD_DCPEVENTGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  class DCPEvent;
}

namespace AIRINV {

  // Forward declarations
  struct DCPEventStruct;
  namespace DCPParserHelper {
    struct doEndDCP;
  }
    
  /** Class handling the generation / instantiation of the DCP BOM. */
  class DCPEventGenerator : public stdair::CmdAbstract {
    // Only the following class may use methods of DCPGenerator.
    // Indeed, as those methods build the BOM, it is not good to expose
    // them public.
    friend class DCPFileParser;
    friend struct DCPParserHelper::doEndDCP;
    friend class DCPParser;
  private:
    /** Generate the DCP objects corresponding to the given
        DCPEvent and add them to the given BomRoot. */
    static void createDCPEvent (stdair::BomRoot&, DCPEventStruct&);
  };

}
#endif // __AIRINV_CMD_DCPEVENTGENERATOR_HPP
