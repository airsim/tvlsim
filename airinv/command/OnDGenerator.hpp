#ifndef __AIRINV_CMD_ONDGENERATOR_HPP
#define __AIRINV_CMD_ONDGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>
#include <stdair/bom/BomAbstract.hpp>
#include <stdair/basic/StructAbstract.hpp>
#include <stdair/bom/AirportPair.hpp>
#include <stdair/stdair_types.hpp>

#include <stdair/bom/SegmentDate.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/BomKeyManager.hpp>
#include <stdair/bom/FlightDateKey.hpp>
#include <stdair/bom/SegmentDateKey.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>


/// Forward declarations

namespace AIRINV {

  class OnDGenerator : public stdair::CmdAbstract {
  public:
    /**
     * Generate the O&D objects.
     */

    static void createOnD (const stdair::BomRoot&);
  };

  
  struct DemandStruct{
  public:
    /** Attributes */
    stdair::Yield_T _yield;
    stdair::NbOfRequests_T _mean;
    stdair::StdDevValue_T _stdDev;
  public:
    /** Constructors and destructors */
    DemandStruct(const stdair::Yield_T& iY, const stdair::NbOfRequests_T& iM,
                 const stdair::StdDevValue_T iSD)
      : _yield(iY), _mean(iM), _stdDev(iSD) {}
    ~DemandStruct() {}
  };

  struct OnDDateKey {
  public:
    /** Constructors and destructors */
    // OnDDateKey(const stdair::AirportCode_T& iO, const stdair::AirportCode_T& iD,
    //          const stdair::Date_T& iDate)
    //   : _origin(iO), _destination(iD), _date(iDate) {}
    OnDDateKey(std::vector<std::string> iKL) : _strList(iKL) {
      assert (!iKL.empty());
      std::string lFK = iKL.front();
      std::string lLK = iKL.back();
      _date = stdair::BomKeyManager::extractFlightDateKey (lFK).getDepartureDate();
      _origin = stdair::BomKeyManager::extractSegmentDateKey (lFK).getBoardingPoint();
      _destination = stdair::BomKeyManager::extractSegmentDateKey (lLK).getOffPoint();      
    }
    OnDDateKey();
  public:
    /**
     * Attributes
     */
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    stdair::Date_T _date;
    std::vector<std::string> _strList;
  };

  class OnDDate {
  public:
    /** Constructors and destructors */
    // OnDDate (const stdair::AirportCode_T& iO, const stdair::AirportCode_T& iD,
    //          const stdair::Date_T& iDate)
    //   : _origin(iO), _destination(iD), _date(iDate) {}
    OnDDate (OnDDateKey iKey)
      : _key(iKey) {}
    ~OnDDate () {}
  public:
    /** Getters */
  public:
    /** Setters */
    void addSegmentDatePtr (stdair::SegmentDate* iSD_ptr) {_segmentDateList.push_back(iSD_ptr);}
    void addClassPathDemand (const std::string& iStr, const DemandStruct& iDS) {
      _classPathDemandMap.insert(std::pair<std::string,DemandStruct> (iStr,iDS));
    }
  public:
    /** Display */
    void display () {
      assert (!_segmentDateList.empty());
      assert (!_classPathDemandMap.empty());
      std::pair<std::string,DemandStruct> lStrDmdPair = *(_classPathDemandMap.begin());
      stdair::SegmentDate* lSegmentDate_ptr = _segmentDateList.front();
      stdair::SegmentDateKey lSegmentDateKey = lSegmentDate_ptr->getKey();
      STDAIR_LOG_DEBUG ("Created O&D date " << _key._origin <<","<<_key._destination
                        <<";" <<_key._date <<" Cabin:Class "<< lStrDmdPair.first
                        << " Yield "<< lStrDmdPair.second._yield
                        << " Demand mean "<< lStrDmdPair.second._mean
                        << " Std deviation "<< lStrDmdPair.second._stdDev);
    }
  public:
    /**
     * Attributes
     */
    // stdair::AirportCode_T _origin;
    // stdair::AirportCode_T _destination;
    // stdair::Date_T _date;
    OnDDateKey _key;
    std::vector<stdair::SegmentDate*> _segmentDateList;
    std::map<std::string, DemandStruct> _classPathDemandMap;
  };

}

#endif // __AIRINV_CMD_ONDGENERATOR_HPP
