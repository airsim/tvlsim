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
    // oCurve[365] = 1.1; oCurve[63] = 1.4;  oCurve[56] = 1.45;
    // oCurve[49] = 1.5;  oCurve[42] = 1.55; oCurve[35] = 1.6;
    // oCurve[31] = 1.7;  oCurve[27] = 1.8;  oCurve[23] = 2.0;
    // oCurve[19] = 2.3;  oCurve[16] = 2.6;  oCurve[13] = 3.0;
    // oCurve[10] = 3.3;  oCurve[7]  = 3.4;  oCurve[5]  = 3.44;
    // oCurve[3] = 3.47;  oCurve[1]  = 3.5;  oCurve[0]  = 3.5;
    // oCurve[365] = 1.0; oCurve[63] = 1.1;  oCurve[56] = 1.13;
    // oCurve[49] = 1.17;  oCurve[42] = 1.22; oCurve[35] = 1.28;
    // oCurve[31] = 1.32;  oCurve[27] = 1.37;  oCurve[23] = 1.43;
    // oCurve[19] = 1.51;  oCurve[16] = 1.6;  oCurve[13] = 1.7;
    // oCurve[10] = 1.8;  oCurve[7]  = 1.9;  oCurve[5]  = 1.93;
    // oCurve[3] = 1.96;  oCurve[1]  = 2.0;  oCurve[0]  = 2.0;
    // oCurve[365] = 1.0; oCurve[63] = 1.05;  oCurve[56] = 1.07;
    // oCurve[49] = 1.09;  oCurve[42] = 1.11; oCurve[35] = 1.14;
    // oCurve[31] = 1.16;  oCurve[27] = 1.18;  oCurve[23] = 1.21;
    // oCurve[19] = 1.24;  oCurve[16] = 1.27;  oCurve[13] = 1.3;
    // oCurve[10] = 1.33;  oCurve[7]  = 1.37;  oCurve[5]  = 1.4;
    // oCurve[3] = 1.45;  oCurve[1]  = 1.5;  oCurve[0]  = 1.5;
    // oCurve[365] = 1.1; oCurve[63] = 1.4; 
    // oCurve[49] = 1.5;  oCurve[35] = 1.6;
    // oCurve[23] = 2.0;  oCurve[16] = 2.6; 
    // oCurve[10] = 3.3;  oCurve[5]  = 3.44;
    // oCurve[1]  = 3.5;  oCurve[0]  = 3.5;
    oCurve[365] = 1.1; oCurve[63] = 1.4; 
    oCurve[49] = 1.5;  oCurve[35] = 2.0;
    oCurve[23] = 2.7;  oCurve[16] = 3.3; 
    oCurve[10] = 3.6;  oCurve[5]  = 3.84;
    oCurve[1]  = 3.9;  oCurve[0]  = 3.9;
    return oCurve;
  };

}
