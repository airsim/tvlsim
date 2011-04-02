#ifndef __AIRSCHED_BOM_ORIGINDESTINATIONSETKEY_HPP
#define __AIRSCHED_BOM_ORIGINDESTINATIONSETKEY_HPP

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
   * @brief Structure representing the key of a sub-network.
   *
   * As the origin airport code is already part of the
   * ReachableUniverse (parent) class, that key is only made of the
   * destination airport code.
   */
  struct OriginDestinationSetKey : public stdair::KeyAbstract {
    friend class boost::serialization::access;

    // /////////// Constructors and destructors ///////////
  private:
    /**
     * Default constructor.
     */
    OriginDestinationSetKey();
    
  public:
    /**
     * Constructor.
     */
    OriginDestinationSetKey (const stdair::AirportCode_T& iDestination);

    /**
     * Copy constructor.
     */
    OriginDestinationSetKey (const OriginDestinationSetKey&);

    /**
     * Destructor.
     */
    ~OriginDestinationSetKey();


  public:
    // /////////// Getters //////////
    /**
     * Get the destination airport.
     */
    const stdair::AirportCode_T& getOffPoint() const {
      return _destination;
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
     * The destination airport.
     */
    stdair::AirportCode_T _destination;
  };

}
#endif // __AIRSCHED_BOM_ORIGINDESTINATIONSETKEY_HPP
