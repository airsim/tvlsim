#ifndef __TRADEMGEN_TRADEMGEN_TYPES_HPP
#define __TRADEMGEN_TRADEMGEN_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace TRADEMGEN {

  // ///////// Exceptions ///////////
  class TrademgenGenerationException : public stdair::RootException {
  };

  class DemandInputFileNotFoundException : public stdair::FileNotFoundException{
  public:
    /** Constructor. */
    DemandInputFileNotFoundException (const std::string& iWhat)
      : stdair::FileNotFoundException (iWhat) {}
  };


  // //////// Type definitions specific to TraDemGen /////////
  // No specific type for now
  
}
#endif // __TRADEMGEN_TRADEMGEN_TYPES_HPP

