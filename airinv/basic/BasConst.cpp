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
    // oCurve[365] = 1.1; oCurve[63] = 1.4; 
    // oCurve[49] = 1.7;  oCurve[48] = 3.6; oCurve[35] = 3.6; oCurve[24] = 3.6;
    // oCurve[23] = 2.6;  oCurve[16] = 2.7; 
    // oCurve[10] = 3.2;  oCurve[5]  = 3.24; oCurve[4]  = 2.8;
    // oCurve[1]  = 2.4;  oCurve[0]  = 2.4;
   
    oCurve[365] = 1.1; oCurve[63] = 1.4;
    /*1*/oCurve[62] = 1.4;  oCurve[56] = 1.45;
    /*2*/oCurve[55] = 1.45; oCurve[49] = 1.5;
    /*3*/oCurve[48] = 1.5;  oCurve[42] = 1.55;
    /*4*/oCurve[41] = 1.95;  oCurve[35] = 2.2;
    /*5*/oCurve[34] = 2.2;  oCurve[31] = 2.4;
    /*6*/oCurve[30] = 2.4;  oCurve[27] = 2.8;
    /*7*/oCurve[26] = 2.9;  oCurve[23] = 3.1;
    /*8*/oCurve[22] = 3.1;  oCurve[19] = 3.3;
    /*9*/oCurve[18] = 3.3;  oCurve[16] = 3.3;
    /*10*/oCurve[15] = 3.3; oCurve[13] = 3.3;
    /*11*/oCurve[12] = 3.0; oCurve[10] = 3.1;
    /*12*/oCurve[9] = 3.1;  oCurve[7]  = 3.1;
    /*13*/oCurve[6] = 3.1; oCurve[5]  = 3.0;
    /*14*/oCurve[4] = 3.1; oCurve[3]  = 3.0;
    /*15*/oCurve[2] = 3.0;  oCurve[1]  = 2.8;
    /*16*/oCurve[0]  = 2.8;

    
    // oCurve[365] = 1.1; oCurve[63] = 1.4;
    // /*1*/oCurve[62] = 1.4;  oCurve[56] = 1.55;
    // /*2*/oCurve[55] = 1.55; oCurve[49] = 1.7;
    // /*3*/oCurve[48] = 3.6;  oCurve[42] = 3.6;
    // /*4*/oCurve[41] = 3.6;  oCurve[35] = 3.6;
    // /*5*/oCurve[34] = 3.6;  oCurve[31] = 3.6;
    // /*6*/oCurve[30] = 3.6;  oCurve[27] = 3.6;
    // /*7*/oCurve[26] = 3.6;  oCurve[23] = 3.6;
    // /*8*/oCurve[22] = 3.5;  oCurve[19] = 3.3;
    // /*9*/oCurve[18] = 3.3;  oCurve[16] = 3.0;
    // /*10*/oCurve[15] = 2.8; oCurve[13] = 2.5;
    // /*11*/oCurve[12] = 2.9; oCurve[10] = 3.2;
    // /*12*/oCurve[9] = 3.2;  oCurve[7]  = 3.22;
    // /*13*/oCurve[6] = 3.25; oCurve[5]  = 3.3;
    // /*14*/oCurve[4] = 3.0; oCurve[3]  = 2.8;
    // /*15*/oCurve[2] = 2.7;  oCurve[1]  = 2.5;
    // /*16*/oCurve[0]  = 2.5;
    

    // oCurve[365] = 1.1; oCurve[63] = 1.4;
    // /*1*/oCurve[62] = 1.4;  oCurve[49] = 1.7;
    // /*2*/oCurve[48] = 3.6;  oCurve[35] = 3.6;
    // /*3*/oCurve[34] = 3.5;  oCurve[23] = 3.4;
    // /*4*/oCurve[22] = 3.3;  oCurve[16] = 3.1;
    // /*5*/oCurve[15] = 2.7; oCurve[10] = 3.1;
    // /*6*/oCurve[9] = 3.0;  oCurve[5]  = 2.8;
    // /*7*/oCurve[4] = 2.3; oCurve[1]  = 2.5;
    // /*8*/oCurve[0]  = 2.5;
    return oCurve;
  };

}
