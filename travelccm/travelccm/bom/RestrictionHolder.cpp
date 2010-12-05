// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// TRAVELCCM
#include <travelccm/bom/TravelSolution.hpp>
#include <travelccm/bom/Restriction.hpp>
#include <travelccm/bom/RestrictionHolder.hpp>

namespace TRAVELCCM {

  // ////////////////////////////////////////////////////////////////////
  RestrictionHolder::RestrictionHolder () {
  }
  
  // ////////////////////////////////////////////////////////////////////
  RestrictionHolder::~RestrictionHolder () {
  }

  // //////////////////////////////////////////////////////////////////////
  void RestrictionHolder::toStream (std::ostream& ioOut) const {
  }

  // //////////////////////////////////////////////////////////////////////
  void RestrictionHolder::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string RestrictionHolder::toString() const {
    std::ostringstream oStr;
    
    RestrictionList_T::const_iterator itRestriction = _restrictionList.begin();
    while (itRestriction != _restrictionList.end()) {
      const Restriction* lRestriction_ptr = *itRestriction;
      assert (lRestriction_ptr != NULL);
      
      oStr << lRestriction_ptr->toString();
      
      ++itRestriction;
    }
    
    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string RestrictionHolder::describeKey() const {
    std::string oKey;

    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string RestrictionHolder::describeShortKey() const {
    std::string oKey;

    return oKey;
  }

   // //////////////////////////////////////////////////////////////////////
   const Restriction& RestrictionHolder::getCurrentRestriction () const {
    const Restriction* resultRestriction_ptr = *_itCurrentRestriction;
    assert (resultRestriction_ptr != NULL);
    
    return (*resultRestriction_ptr);
  }

  ///////////////////////////////////////////////////////////////////////
  void RestrictionHolder::addRestriction (const Restriction& iRestriction) {
    _restrictionList.push_back (&iRestriction);
  }

  // //////////////////////////////////////////////////////////////////////
  void RestrictionHolder::begin () {
    _itCurrentRestriction = _restrictionList.begin();
  }

  // //////////////////////////////////////////////////////////////////////
  bool RestrictionHolder::hasNotReachedEnd () const {
    bool result = (_itCurrentRestriction != _restrictionList.end());
    return result;
  }

  // //////////////////////////////////////////////////////////////////////
  void RestrictionHolder::iterate () {
    if (_itCurrentRestriction != _restrictionList.end()) {
      _itCurrentRestriction++;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void RestrictionHolder::eraseCurrentRestriction () {
    assert (_itCurrentRestriction != _restrictionList.end());
    _itCurrentRestriction = _restrictionList.erase (_itCurrentRestriction);
  }

}
