#ifndef __SIMFQT_BOM_FAREPOSITIONKEY_HPP
#define __SIMFQT_BOM_FAREPOSITIONKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// SIMFQT
#include <stdair/bom/KeyAbstract.hpp>
#include <stdair/stdair_basic_types.hpp>

namespace SIMFQT  {
  /** Key of fare position. */
  struct FarePositionKey : public stdair::KeyAbstract {

  private:
    // /////////// Default constructor //////////
    FarePositionKey () { };
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    FarePositionKey (const stdair::CityCode_T&);
    FarePositionKey (const FarePositionKey&);
    /** Destructor. */
    ~FarePositionKey ();
    
    // /////////// Getter //////////
    /** Get the position. */
    const stdair::CityCode_T& getPosition() const {
      return _position;
    }
    
    // /////////// Display support methods /////////
    /** Dump a Business Object Key into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object Key from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

   /** Get the serialised version of the Business Object Key.
       <br>That string is unique, at the level of a given Business Object,
       when among children of a given parent Business Object.*/
    const std::string toString() const;

  private:
    // Attributes
    /** City code position */
    stdair::CityCode_T _position;

  };

}
#endif // __SIMFQT_BOM_FAREPOSITIONKEY_HPP
