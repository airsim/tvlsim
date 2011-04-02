#ifndef __AIRSCHED_BOM_ORIGINDESTINATIONSET_HPP
#define __AIRSCHED_BOM_ORIGINDESTINATIONSET_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// StdAir
#include <stdair/bom/BomAbstract.hpp>
// AirSched
#include <airsched/bom/OriginDestinationSetKey.hpp>
#include <airsched/bom/OriginDestinationSetTypes.hpp>

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
}

namespace AIRSCHED {

  /**
   * @brief Class representing a simple sub-network.
   *
   * That simple sub-network is made of a set of segments
   * (SegmentPathPeriod objects), from the origin airport specified
   * within ReachableUniverse (parent object) to the destination
   * specified in the OriginDestinationSetKey object.
   *
   * Each segment (composing that OriginDestinationSet object)
   * corresponds to an actual travel solution from the origin to the
   * destination, that is, a path that a traveller can take with
   * actual scheduled flights.
   */
  class OriginDestinationSet : public stdair::BomAbstract {
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
    typedef OriginDestinationSetKey Key_T;


  public:
    // /////////// Getters /////////////
    /**
     * Get the primary key (destination airport).
     */
    const Key_T& getKey() const {
      return _key;
    }

    /**
     * Get the destination airport (i.e., the primary key).
     */
    const stdair::AirportCode_T& getDestination() const {
      return _key.getOffPoint();
    }

    /**
     * Get the parent (i.e., ReachableUniverse) object.
     */
    stdair::BomAbstract* const getParent() const {
      return _parent;
    }

    /**
     * Get the map of children holders (SegmentPathPeriod objects).
     */
    const stdair::HolderMap_T& getHolderMap() const {
      return _holderMap;
    }


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
    OriginDestinationSet (const Key_T&);

    /**
     * Destructor.
     */
    ~OriginDestinationSet();

  private:
    /**
     * Default constructor.
     */
    OriginDestinationSet();

    /**
     * Copy constructor.
     */
    OriginDestinationSet (const OriginDestinationSet&);

  protected:
    // ////////// Attributes /////////
    /**
     * Primary key (destination airport code).
     */
    Key_T _key;

    /**
     * Pointer on the parent (ReachableUniverse) object.
     */
    stdair::BomAbstract* _parent;

    /**
     * Map holding the children (SegmentPathPeriod objects).
     */
    stdair::HolderMap_T _holderMap;
  };

}
#endif // __AIRSCHED_BOM_ORIGINDESTINATIONSET_HPP

