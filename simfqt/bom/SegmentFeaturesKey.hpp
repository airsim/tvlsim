#ifndef __SIMFQT_BOM_SEGMENTFEATURESKEY_HPP
#define __SIMFQT_BOM_SEGMENTFEATURESKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <list>
// SIMFQT
#include <stdair/bom/KeyAbstract.hpp>
#include <stdair/stdair_types.hpp>

namespace SIMFQT  {
  /** Key of airport-pair. */
  struct SegmentFeaturesKey : public stdair::KeyAbstract {

  private:
    // /////////// Default constructor //////////
    SegmentFeaturesKey () { };
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    SegmentFeaturesKey (const stdair::AirlineCodeList_T&,
                        const stdair::ClassList_StringList_T&);
    SegmentFeaturesKey (const SegmentFeaturesKey&);
    /** Destructor. */
    ~SegmentFeaturesKey ();
    
    // /////////// Getters //////////
    /** Get the airline code list. */
    const stdair::AirlineCodeList_T& getAirlineCodeList() const {
      return _airlineCodeList;
    }

    /** Get the class code list. */
    const stdair::ClassList_StringList_T& getClassCodeList() const {
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
    /** Airline code List. */
    stdair::AirlineCodeList_T _airlineCodeList;

    /** Classe code List. */
    stdair::ClassList_StringList_T _classCodeList;
  };

}
#endif // __SIMFQT_BOM_SEGMENTFEATURESKEY_HPP
