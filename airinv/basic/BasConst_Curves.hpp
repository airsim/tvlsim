#ifndef __AIRINV_BAS_BASCONST_CURVES_HPP
#define __AIRINV_BAS_BASCONST_CURVES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// AIRINV
#include <airinv/AIRINV_Types.hpp>

namespace AIRINV {

  /** Default pick-up FRAT5 curve for Q-equivalent booking conversion. */
  extern const FRAT5Curve_T DEFAULT_PICKUP_FRAT5_CURVE;

  /** Default PoS probability mass. */
  struct DefaultMap {
    static FRAT5Curve_T createPickupFRAT5Curve();
  };
}
#endif // __AIRINV_BAS_BASCONST_CURVES_HPP
