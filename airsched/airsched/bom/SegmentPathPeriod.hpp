#ifndef __AIRSCHED_BOM_SEGMENTPATHPERIOD_HPP
#define __AIRSCHED_BOM_SEGMENTPATHPERIOD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR 
#include <stdair/bom/BomAbstract.hpp>
// AIRSCHED
#include <airsched/bom/SegmentPathPeriodKey.hpp>
#include <airsched/bom/SegmentPathPeriodTypes.hpp>

// Forward declaration
namespace stdair {
  class SegmentPeriod;
  template <typename BOM> class FacBom;
  class FacBomManager;
}

namespace AIRSCHED {
  /** Class representing the actual attributes for an airline outbound class. */
  class SegmentPathPeriod : public stdair::BomAbstract {
    template <typename BOM> friend class stdair::FacBom;
    friend  class stdair::FacBomManager;

  public:
    // Type definitions.
    /** Definition allowing to retrieve the associated BOM key type. */
    typedef SegmentPathPeriodKey Key_T;

  public:
    // /////////// Getters ////////////
    /** Get the outbound path key. */
    const Key_T& getKey() const { return _key; }

    /** Get the parent object. */
    stdair::BomAbstract* const getParent() const { return _parent; }

    /** Get the number of segments (part of the primary key). */
    const stdair::NbOfSegments_T getNbOfSegments() const {
      return _key.getNbOfSegments();
    }

    /** Get the boarding date offset list. */
    const DateOffsetList_T& getBoardingDateOffsetList () const {
      return _key.getBoardingDateOffsetList();
    }

    /** Get the elapsed time (part of the primary key). */
    const stdair::Duration_T& getElapsedTime() const {
      return _key.getElapsedTime();
    }

    /** Get the number of airlines (part of the primary key). */
    const stdair::NbOfAirlines_T& getNbOfAirlines() const {
      return _key.getNbOfAirlines();
    }

    /** Get the boarding time. */
    const stdair::Duration_T& getBoardingTime() const {
      return _key.getBoardingTime();
    }

    /** Get the departure period. */
    const stdair::PeriodStruct& getDeparturePeriod () const {
      return _key.getPeriod();
    }

    /** Get the map of children holders. */
    const stdair::HolderMap_T& getHolderMap() const { return _holderMap; }

    /** Get the last SegmentPeriod object of the list.
        <br>Return a NULL pointer if the list is empty. */
    stdair::SegmentPeriod* getLastSegmentPeriod () const;

    /** Get the first SegmentPeriod object of the list.
        <br>Return a NULL pointer if the list is empty. */
    stdair::SegmentPeriod* getFirstSegmentPeriod () const;

    /** Get the destination of the segment path (i.e., the destination
        of the last segment. */
    const stdair::AirportCode_T& getDestination () const;
    
  public:
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const { ioOut << toString(); }

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn) { }

   /** Get the serialised version of the Business Object. */
    std::string toString() const;
    
    /** Get a string describing the  key. */
    const std::string describeKey() const { return _key.toString(); }

  public:
    // ////////////// Busniess methods ////////////////
    /** Check if the (i-1)-length segment path period can be fused with the
        single segment segment path period in order to create an i-length
        segment path period. The function will return a valid or non-valid
        segment path period key.
        <br>The two segment path period above can be fused (and will produce a
        valid new segment path period key) if:
        1. A passenger can connect from the last segment of the
        first segment path and the first segment of the next segment path.
        These two segments should not create another segment.
        2. There is no circle within the new segment path.
        3. The intersection of the two periods is non-empty.
    */
    Key_T connectWithAnotherSegment (const SegmentPathPeriod&) const;

    /** Check if the given destination airport the departure point of
        one of the segment member. If yes, a circle exists. */
    bool checkCircle (const stdair::AirportCode_T&) const;

    /** State whether or not the given airline is flown by (at least) one of
        the segments of the internal list. */
    bool isAirlineFlown (const stdair::AirlineCode_T&) const;

    /** Check if the given departure date is included in the departure period
        of the segment path. */
    bool isDepartureDateValid (const stdair::Date_T&) const;
    
  protected:
    /** Default constructors. */
    SegmentPathPeriod (const Key_T&);
    SegmentPathPeriod (const SegmentPathPeriod&);
    /** Destructor. */
    ~SegmentPathPeriod();

  protected:
    // Attributes
    Key_T _key;
    stdair::BomAbstract* _parent;
    stdair::HolderMap_T _holderMap;
  };

}
#endif // __AIRSCHED_BOM_SEGMENTPATHPERIOD_HPP

