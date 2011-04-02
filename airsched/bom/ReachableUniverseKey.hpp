#ifndef __AIRSCHED_BOM_REACHABLEUNIVERSEKEY_HPP
#define __AIRSCHED_BOM_REACHABLEUNIVERSEKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/KeyAbstract.hpp>

/// Forward declarations
namespace boost {
  namespace serialization {
    class access;
  }
}

namespace AIRSCHED {

  /**
   * @brief Structure representing the key of the schedule-related BOM
   *        tree root.
   *
   * The ReachableUniverse is the pending, in the schedule universe,
   * of the stdair::Inventory class. It corresponds to all the
   * destinations which can be reached from a given geographical
   * point. That latter is an airport for now, and the present
   * structure specifies its key (i.e., airport code).
   */
  struct ReachableUniverseKey : public stdair::KeyAbstract {
    friend class boost::serialization::access;

    // /////////// Constructors and destructors ///////////
  private:
    /**
     * Default constructor.
     */
    ReachableUniverseKey();
    
  public:
    /**
     * Constructor.
     */
    ReachableUniverseKey (const stdair::AirportCode_T& iOrigin);

    /**
     * Copy constructor.
     */
    ReachableUniverseKey (const ReachableUniverseKey&);

    /**
     * Destructor.
     */
    ~ReachableUniverseKey();


  public:
    // /////////// Getters //////////
    /**
     * Get the origin airport (from which the remaining universe may
     * be reached).
     */
    const stdair::AirportCode_T& getBoardingPoint() const {
      return _origin;
    }


  public:
    // /////////// Display support methods /////////
    /**
     * Dump a Business Object Key into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const;

    /**
     * Read a Business Object Key from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);

    /**
     * Get the serialised version of the Business Object Key.
     *
     * That string is unique, at the level of a given Business Object,
     * when among children of a given parent Business Object.
     *
     * For instance, "H" and "K" allow to differentiate among two
     * marketing classes for the same segment-date.
     */
    const std::string toString() const;


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


  private:
    // ///////////////// Attributes ///////////////
    /**
     * The origin airport (from which the remaining universe may
     * be reached).
     */
    stdair::AirportCode_T _origin;
  };

}

#endif // __AIRSCHED_BOM_REACHABLEUNIVERSEKEY_HPP
