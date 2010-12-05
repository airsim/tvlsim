// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// TRAVEL-CCM
#include <travelccm/bom/BomAbstract.hpp>
#include <travelccm/factory/FacBomAbstract.hpp>

namespace TRAVELCCM {
  
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

}
