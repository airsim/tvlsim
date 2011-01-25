#ifndef __SIMFQT_BOM_FAREPOSCHANNELKEY_HPP
#define __SIMFQT_BOM_FAREPOSCHANNELKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// SIMFQT
#include <stdair/bom/KeyAbstract.hpp>
#include <stdair/stdair_types.hpp>

namespace SIMFQT  {
  /** Key of fare position. */
  struct FarePosChannelKey : public stdair::KeyAbstract {

  private:
    // /////////// Default constructor //////////
    FarePosChannelKey () { };
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    FarePosChannelKey (const stdair::CityCode_T&, const stdair::ChannelLabel_T&);
    FarePosChannelKey (const FarePosChannelKey&);
    /** Destructor. */
    ~FarePosChannelKey ();
    
    // /////////// Getter //////////
    /** Get the position. */
    const stdair::CityCode_T& getPosition() const {
      return _position;
    }
   
    /** Get the channel. */
    const stdair::ChannelLabel_T& getChannel() const {
      return _channel;
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

    /** Booking channel
        (D=direct, I=indirect, N=oNline, F=oFfline). */
    stdair::ChannelLabel_T _channel;

  };

}
#endif // __SIMFQT_BOM_FAREPOSCHANNELKEY_HPP
