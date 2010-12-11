// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost (STL Extension)
#include <boost/functional/hash/hash.hpp>
// Simcrs
#include <simcrs/bom/BomAbstract.hpp>
#include <simcrs/factory/FacBomAbstract.hpp>

namespace SIMCRS {
  
  // //////////////////////////////////////////////////////////////////////
  FacBomAbstract::~FacBomAbstract() {
    clean ();
  }

  // //////////////////////////////////////////////////////////////////////
  void FacBomAbstract::clean() {
    for (BomPool_T::iterator itBom = _pool.begin();
	 itBom != _pool.end(); itBom++) {
      BomAbstract* currentBom_ptr = *itBom;
      assert (currentBom_ptr != NULL);

      delete (currentBom_ptr); currentBom_ptr = NULL;
    }

    // Empty the pool of Factories
    _pool.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::size_t FacBomAbstract::getID (const BomAbstract* iBomAbstract_ptr) {
    const void* lPtr = iBomAbstract_ptr;
    boost::hash<const void*> ptr_hash;
    const std::size_t lID = ptr_hash (lPtr);
    return lID;
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::size_t FacBomAbstract::getID (const BomAbstract& iBomAbstract) {
    return getID (&iBomAbstract);
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string FacBomAbstract::getIDString(const BomAbstract* iBomAbstract_ptr) {
    const std::size_t lID = getID (iBomAbstract_ptr);
    std::ostringstream oStr;
    oStr << lID;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string FacBomAbstract::getIDString (const BomAbstract& iBomAbstract) {
    return getIDString (&iBomAbstract);
  }

}
