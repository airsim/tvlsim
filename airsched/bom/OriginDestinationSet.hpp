#ifndef __AIRSCHED_BOM_ORIGINDESTINATIONSET_HPP
#define __AIRSCHED_BOM_ORIGINDESTINATIONSET_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/bom/BomAbstract.hpp>
// AIRSCHED
#include <airsched/bom/OriginDestinationSetKey.hpp>
#include <airsched/bom/OriginDestinationSetTypes.hpp>

namespace stdair {
  template <typename BOM> class FacBom;
  class FacBomManager;
}

namespace AIRSCHED {

  /** Class representing the actual attributes for an origin-destination set. */
  class OriginDestinationSet : public stdair::BomAbstract {
    template <typename BOM> friend class stdair::FacBom;
    friend  class stdair::FacBomManager;

  public :
    // Type definitions
    /** Definition allowing to retrieve the associated BOM key type. */
    typedef OriginDestinationSetKey Key_T;

  public:
    // ////////// Getters ////////////
    /** Get the network key. */
    const Key_T& getKey() const { return _key; }

    /** Get the parent object. */
    stdair::BomAbstract* const getParent() const { return _parent; }

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

  protected:
    /** Default constructors. */
    OriginDestinationSet (const Key_T&);
    OriginDestinationSet (const OriginDestinationSet&);
    /** Destructor. */
    ~OriginDestinationSet();

  protected:
    // Attributes
    Key_T _key;
    stdair::BomAbstract* _parent;
    stdair::HolderMap_T _holderMap;
  };

}
#endif // __AIRSCHED_BOM_ORIGINDESTINATIONSET_HPP

