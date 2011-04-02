#ifndef __AIRSCHED_BOM_SEGMENTPATHPERIOD_HPP
#define __AIRSCHED_BOM_SEGMENTPATHPERIOD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// StdAir
#include <stdair/bom/BomAbstract.hpp>
// AirSched
#include <airsched/bom/SegmentPathPeriodKey.hpp>
#include <airsched/bom/SegmentPathPeriodTypes.hpp>

/// Forward declarations
namespace boost {
  namespace serialization {
    class access;
  }
}

/// Forward declarations
namespace stdair {
  template <typename BOM> class FacBom;
  class FacBomManager;
  class SegmentPeriod;
}

namespace AIRSCHED {

  /**
   * @brief Class representing a segment/path.
   *
   * It corresponds to an actual travel solution from the origin to
   * the destination, that is, a path that a traveller can take with
   * actual scheduled flights.
   */
  class SegmentPathPeriod : public stdair::BomAbstract {
    /**
     * Friend classes.
     */
    template <typename BOM> friend class stdair::FacBom;
    friend  class stdair::FacBomManager;
    friend class boost::serialization::access;

  public:
    // ////////// Type definitions ////////////
    /**
     * Definition allowing to retrieve the associated BOM key type.
     */
    typedef SegmentPathPeriodKey Key_T;


  public:
    // /////////// Getters /////////////
    /**
     * Get the primary key (destination airport).
     */
    const Key_T& getKey() const {
      return _key;
    }

    /**
     * Get the parent (i.e., OriginDestinationSet) object.
     */
    stdair::BomAbstract* const getParent() const {
      return _parent;
    }

    /** Get the departure period (part of the primary key). */
    const stdair::PeriodStruct& getDeparturePeriod() const {
      return _key.getPeriod();
    }

    /** Get the boarding date offset list (part of the primary key). */
    const DateOffsetList_T& getBoardingDateOffsetList () const {
      return _key.getBoardingDateOffsetList();
    }

    /** Get the number of segments (part of the primary key). */
    const stdair::NbOfSegments_T getNbOfSegments() const {
      return _key.getNbOfSegments();
    }

    /** Get the number of airlines (part of the primary key). */
    const stdair::NbOfAirlines_T& getNbOfAirlines() const {
      return _key.getNbOfAirlines();
    }

    /** Get the elapsed time (part of the primary key). */
    const stdair::Duration_T& getElapsedTime() const {
      return _key.getElapsedTime();
    }

    /** Get the boarding time (part of the primary key). */
    const stdair::Duration_T& getBoardingTime() const {
      return _key.getBoardingTime();
    }

    /**
     * Get the map of children holders (SegmentPeriod objects).
     */
    const stdair::HolderMap_T& getHolderMap() const {
      return _holderMap;
    }

    /**
     * Get the last SegmentPeriod object of the list.
     *
     * Return a NULL pointer if the list is empty.
     */
    stdair::SegmentPeriod* getLastSegmentPeriod() const;

    /**
     * Get the first SegmentPeriod object of the list.
     *
     * Return a NULL pointer if the list is empty.
     */
    stdair::SegmentPeriod* getFirstSegmentPeriod() const;

    /**
     * Get the destination of the segment path (i.e., the destination
     * of the last segment.
     */
    const stdair::AirportCode_T& getDestination() const;


  public:
    // ////////////// Business methods ////////////////
    /**
     * Check whether the (i-1)-length segment path period can be merged
     * with the single segment path period in order to create an
     * i-length segment path period. The function will return a valid
     * or non-valid segment path period key.
     *
     * The two segment path period above can be fused (and will produce a
     * valid new segment path period key) if:
     * <ol>
     *  <li>A passenger can connect from the last segment of the
     *      first segment path and the first segment of the next segment path.
     *      These two segments should not create another segment.
     *  </li>
     *  <li>There is no circle within the new segment path.</li>
     *  <li>The intersection of the two periods is non-empty.</li>
     * </ol>
     */
    Key_T connectWithAnotherSegment (const SegmentPathPeriod&) const;

    /**
     * Check whether the given destination airport is also the
     * departure point of one of the other segment members. If yes, a
     * circle exists.
     */
    bool checkCircle (const stdair::AirportCode_T&) const;

    /**
     * State whether or not the given airline is flown by (at least)
     * one of the segments of the internal list.
     */
    bool isAirlineFlown (const stdair::AirlineCode_T&) const;

    /**
     * Check whether the given departure date is included in the
     * departure period of the segment path.
     */
    bool isDepartureDateValid (const stdair::Date_T&) const;
    
  public:
    // /////////// Display support methods /////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const {
      ioOut << toString();
    }

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn) {
    }

    /**
     * Get the serialised version of the Business Object.
     */
    std::string toString() const;
    
    /**
     * Get a string describing the  key.
     */
    const std::string describeKey() const {
      return _key.toString();
    }
    

  public:
    // /////////// (Boost) Serialisation support methods /////////
    /**
     * Serialisation.
     */
    template<class Archive>
    void serialize (Archive& ar, const unsigned int iFileVersion);

  private:
    /**
     * Serialisation helper (allows to be sure the template method is
     * instantiated).
     */
    void serialisationImplementation();


  protected:
    // ////////// Constructors and destructors /////////
    /**
     * Main constructor.
     */
    SegmentPathPeriod (const Key_T&);

    /**
     * Destructor.
     */
    ~SegmentPathPeriod();

  private:
    /**
     * Default constructor.
     */
    SegmentPathPeriod();

    /**
     * Copy constructor.
     */
    SegmentPathPeriod (const SegmentPathPeriod&);


  protected:
    // ////////// Attributes /////////
    /**
     * Primary key (segment/path characteristics: scheduled period,
     * number of segments, number of airlines, elapsed time, boarding
     * time).
     */
    Key_T _key;

    /**
     * Pointer on the parent (OriginDestinationSet) object.
     */
    stdair::BomAbstract* _parent;

    /**
     * Map holding the children (SegmentPeriod objects).
     *
     * \note The SegmentPeriod objects themselves have for parent the
     * FlightPeriod class (not the SegmentPathPeriod class).
     */
    stdair::HolderMap_T _holderMap;
  };

}
#endif // __AIRSCHED_BOM_SEGMENTPATHPERIOD_HPP

