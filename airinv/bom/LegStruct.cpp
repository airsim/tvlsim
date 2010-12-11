// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// STDAIR
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/LegDate.hpp>
// AIRINV
#include <airinv/bom/LegStruct.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  LegStruct::LegStruct ()
    : _boardingDate (stdair::DEFAULT_DATE), _offDate (stdair::DEFAULT_DATE) {
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string LegStruct::describe() const {
    std::ostringstream ostr;
    ostr << "    " << _boardingPoint << " / " << _boardingDate << " "
         << boost::posix_time::to_simple_string(_boardingTime)
         << " -- " << _offPoint << " / " << _offDate << " "
         << boost::posix_time::to_simple_string(_offTime)
         << " --> "
         << boost::posix_time::to_simple_string(_elapsed)
         << std::endl;
    for (LegCabinStructList_T::const_iterator itCabin = _cabinList.begin();
         itCabin != _cabinList.end(); itCabin++) {
      const LegCabinStruct& lCabin = *itCabin;
      ostr << lCabin.describe();
    }
    ostr << std::endl;
    
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void LegStruct::fill (const stdair::Date_T& iRefDate,
                          stdair::LegDate& ioLegDate) const {
    // Set the Off Point
    ioLegDate.setOffPoint (_offPoint);
    // Set the Boarding Date
    ioLegDate.setBoardingDate (iRefDate + _boardingDateOffset);
    // Set the Boarding Time
    ioLegDate.setBoardingTime (_boardingTime);
    // Set the Off Date
    ioLegDate.setOffDate (iRefDate + _offDateOffset);
    // Set the Off Time
    ioLegDate.setOffTime (_offTime);
    // Set the Elapsed Time
    ioLegDate.setElapsedTime (_elapsed);
  }

  // //////////////////////////////////////////////////////////////////////
  void LegStruct::fill (stdair::LegDate& ioLegDate) const {
    // Set the Off Point
    ioLegDate.setOffPoint (_offPoint);
    // Set the Boarding Date
    ioLegDate.setBoardingDate (_offDate);
    // Set the Boarding Time
    ioLegDate.setBoardingTime (_boardingTime);
    // Set the Off Date
    ioLegDate.setOffDate (_offDate);
    // Set the Off Time
    ioLegDate.setOffTime (_offTime);
    // Set the Elapsed Time
    ioLegDate.setElapsedTime (_elapsed);
  }

}
