#ifndef __TRAVELCCM_CMD_FILEMGR_HPP
#define __TRAVELCCM_CMD_FILEMGR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <string>

namespace TRAVELCCM {

  class TravelSolutionHolder;
  
  /** Class filling the TravelSolutionHolder structure (representing
      a list of classes/travelSolutions) from a given input file. */
  class FileMgr {
  public:
    /** Read the input values from a CSV file. */
    static void
    readAndProcessTravelSolutionInputFile (const std::string& iInputFileName,
                                           TravelSolutionHolder&);
    
  };
}
#endif // __TRAVELCCM_CMD_FILEMGR_HPP
