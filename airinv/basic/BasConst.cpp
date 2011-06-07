// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <airinv/basic/BasConst_General.hpp>
#include <airinv/basic/BasConst_Curves.hpp>
#include <airinv/basic/BasConst_AIRINV_Service.hpp>

namespace AIRINV {

  /** Default airline name for the AIRINV_Service. */
  const std::string DEFAULT_AIRLINE_CODE = "BA";

  /** Default pick-up FRAT5 curve for Q-equivalent booking conversion. */
  const FRAT5Curve_T DEFAULT_PICKUP_FRAT5_CURVE =
    DefaultMap::createPickupFRAT5Curve();
  FRAT5Curve_T DefaultMap::createPickupFRAT5Curve() {
    FRAT5Curve_T oCurve;
    oCurve[365] = 1.1; oCurve[63] = 1.4;  oCurve[56] = 1.45;
    oCurve[49] = 1.5;  oCurve[42] = 1.55; oCurve[35] = 1.6;
    oCurve[31] = 1.7;  oCurve[28] = 1.8;  oCurve[24] = 2.0;
    oCurve[21] = 2.3;  oCurve[17] = 2.6;  oCurve[14] = 3.0;
    oCurve[10] = 3.3;  oCurve[7]  = 3.4;  oCurve[5]  = 3.44;
    oCurve[3] = 3.47;  oCurve[1]  = 3.5;
    return oCurve;
  };

}
