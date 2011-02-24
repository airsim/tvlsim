#ifndef __SIMFQT_BOM_SEGMENTFEATURESKEY_HPP
#define __SIMFQT_BOM_SEGMENTFEATURESKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <list>
// SIMFQT
#include <stdair/bom/KeyAbstract.hpp>
#include <stdair/stdair_basic_types.hpp>

namespace SIMFQT  {
  /** Key of airport-pair. */
  struct SegmentFeaturesKey : public stdair::KeyAbstract {

  private:
    // /////////// Default constructor //////////
    SegmentFeaturesKey () { };
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    SegmentFeaturesKey (const stdair::AirlineCode_T&, const std::list<std::string> &);
    SegmentFeaturesKey (const SegmentFeaturesKey&);
    /** Destructor. */
    ~SegmentFeaturesKey ();
    
    // /////////// Getters //////////
    /** Get the airline code. */
    const stdair::AirlineCode_T& getAirlineCode() const {
      return _airlineCode;
    }

    /** Get the class code. */
    const std::list<std::string>& getClassCodeList() const {
      return _classCodeList;
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
       when among children of a given parent Business Object. */
    const std::string toString() const;

  private:
    // Attributes
    /** Airline code. */
    stdair::AirlineCode_T _airlineCode;

    /** Classe code. */
    std::list<std::string> _classCodeList;
  };

}
#endif // __SIMFQT_BOM_SEGMENTFEATURESKEY_HPP
