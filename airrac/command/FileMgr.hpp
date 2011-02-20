#ifndef __AIRSCHED_CMD_FILEMGR_HPP
#define __AIRSCHED_CMD_FILEMGR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <string>

namespace AIRSCHED {

  /** Class filling the TravelSolutionHolder structure (representing
      a list of classes/travelSolutions) from a given input file. */
  class FileMgr {
  public:
    /** Read the input values from a CSV file. */
    static void
    readAndProcessTravelSolutionInputFile (const std::string& iInputFileName);
    
  };
}
#endif // __AIRSCHED_CMD_FILEMGR_HPP
