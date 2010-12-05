#ifndef __AIRSCHED_BOM_REACHABLEUNIVERSE_HPP
#define __AIRSCHED_BOM_REACHABLEUNIVERSE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR 
#include <stdair/bom/BomAbstract.hpp>
// AIRSCHED
#include <airsched/bom/ReachableUniverseKey.hpp>
#include <airsched/bom/ReachableUniverseTypes.hpp>
#include <airsched/bom/SegmentPathPeriodTypes.hpp>

namespace stdair {
  template <typename BOM> class FacBom;
  class FacBomManager;
}

namespace AIRSCHED {

  /** Class representing the actual attributes for an airport-date. */
  class ReachableUniverse : public stdair::BomAbstract {
    template <typename BOM> friend class stdair::FacBom;
    friend  class stdair::FacBomManager;

  public:
    // Type definitions.
    /** Definition allowing to retrieve the associated BOM key type. */
    typedef ReachableUniverseKey Key_T;

  public:
    // /////////// Getters /////////////
    /** Get the airport-date key. */
    const Key_T& getKey() const { return _key; }

    /** Get the parent object. */
    stdair::BomAbstract* const getParent() const { return _parent; }
    
    /** Get the (origin) airport (part of the primary key). */
    const stdair::AirportCode_T& getOrigin() const {
      return _key.getBoardingPoint();
    }

    /** Get the SegmentPathPeriodListList. */
    const SegmentPathPeriodListList_T& getSegmentPathPeriodListList () const {
      return _segmentPathPeriodListList;
    }

    /** Get the map of children holders. */
    const stdair::HolderMap_T& getHolderMap() const { return _holderMap; }

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
    // //////////// Business Methods //////////////
    /** Add a segment path period to the dedicated list. **/
    void addSegmentPathPeriod (const SegmentPathPeriod&);    
           
  protected:
    /** Default constructors. */
    ReachableUniverse (const Key_T&);
    ReachableUniverse (const ReachableUniverse&);
    /** Destructor. */
    ~ReachableUniverse();

  protected:
    // Attributes
    Key_T _key;
    stdair::BomAbstract* _parent;
    stdair::HolderMap_T _holderMap;
    
    /** The list of lists of SegmentPathPeriods, used uniquement for the
        construction of the main list of SegmentPathPeriods in
        ReachableUniverseStructure. */
    SegmentPathPeriodListList_T _segmentPathPeriodListList;
  };

}
#endif // __AIRSCHED_BOM_REACHABLEUNIVERSE_HPP

