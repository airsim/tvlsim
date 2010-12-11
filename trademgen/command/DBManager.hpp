#ifndef __TRADEMGEN_CMD_DBMANAGER_HPP
#define __TRADEMGEN_CMD_DBMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_db.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Types.hpp>

namespace TRADEMGEN {

  // Forward declarations
  struct AirlineStruct;

  /** Class building the Business Object Model (BOM) from data retrieved
      from the database. */
  class DBManager {
  public:
    /** Update the fields of the database row
        corresponding to the given BOM object. */
    static void updateAirlineInDB (stdair::DBSession_T&,
                                   const stdair::AirlineStruct&);

    /** Retrieve, from the (MySQL) database, the row corresponding to
        the given BOM code, and fill the given BOM object with that retrieved
        data. */
    static bool retrieveAirline (stdair::DBSession_T&,
                                 const stdair::AirlineCode_T&,
                                 stdair::AirlineStruct&);

    
  public:
    /** Prepare (parse and put in cache) the SQL statement. */
    static void prepareSelectStatement (stdair::DBSession_T&,
                                        stdair::DBRequestStatement_T&,
                                        stdair::AirlineStruct&);

    /** Iterate on the SQL statement.
        <br>The SQL has to be already prepared.
        @parameter const bool Tells whether the Airline object should be
                   reset. */
    static bool iterateOnStatement (stdair::DBRequestStatement_T&,
                                    stdair::AirlineStruct&,
                                    const bool iShouldDoReset);

    
  private:
    /** Prepare (parse and put in cache) the SQL statement. */
    static void prepareSelectOnAirlineCodeStatement (stdair::DBSession_T&,
                                                     stdair::DBRequestStatement_T&,
                                                     const stdair::AirlineCode_T&,
                                                     stdair::AirlineStruct&);

    
  private:
    /** Constructors. */
    DBManager() {}
    DBManager(const DBManager&) {}
    /** Destructor. */
    ~DBManager() {}
  };
  
}
#endif // __TRADEMGEN_CMD_DBMANAGER_HPP
