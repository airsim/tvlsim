#ifndef __TRADEMGEN_TRADEMGEN_EXCEPTIONS_HPP
#define __TRADEMGEN_TRADEMGEN_EXCEPTIONS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace TRADEMGEN {

  // ///////// Exceptions ///////////
  /**
   * Root exception for the TraDemGen component
   */
  class TrademgenGenerationException : public stdair::RootException {
  public:
    /**
     * Constructor.
     */
    TrademgenGenerationException (const std::string& iWhat)
      : stdair::RootException (iWhat) {}
  };

  /**
   * Exception when no demand input file can be found
   */
  class DemandInputFileNotFoundException
    : public stdair::FileNotFoundException {
  public:
    /**
     * Constructor.
     */
    DemandInputFileNotFoundException (const std::string& iWhat)
      : stdair::FileNotFoundException (iWhat) {}
  };

  /**
   * Exception when index out of range
   */
  class IndexOutOfRangeException : public TrademgenGenerationException {
  public:
    /**
     * Constructor.
     */
    IndexOutOfRangeException (const std::string& iWhat)
      : TrademgenGenerationException (iWhat) {}
  };

}
#endif // __TRADEMGEN_TRADEMGEN_EXCEPTIONS_HPP

