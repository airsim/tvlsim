#ifndef __TVLSIM_BOM_BOMJSONEXPORT_HPP
#define __TVLSIM_BOM_BOMJSONEXPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// Boost Property Tree
#if BOOST_VERSION >= 104100
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#endif // BOOST_VERSION >= 104100

#if BOOST_VERSION >= 104100
  namespace bpt = boost::property_tree;
#else // BOOST_VERSION >= 104100
  namespace bpt {
    typedef char ptree;
  }
#endif // BOOST_VERSION >= 104100

namespace TVLSIM {

  struct SimulationStatus;

  /**
   * @brief Utility class to export StdAir objects in a JSON format.
   */
  
  class BomJSONExport {
  public:
    // //////////////// Export support methods /////////////////   

    /**
     * Export (dump in the underlying output log stream and in JSON format) 
     * the simulation status
     *
     * @param const SimulationStatus& Simulation status to be stored in JSON-ified 
     *        format.
     */
    static void jsonExportSimulationStatus (std::ostream&, 
					    const SimulationStatus&);
    
  };

}
#endif // __TVLSIM_BOM_BOMJSONEXPORT_HPP
