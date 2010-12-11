// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// SOCI
#include <soci/core/soci.h>
#include <soci/backends/mysql/soci-mysql.h>
// StdAir
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/command/DBManager.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectStatement (stdair::DBSession_T& ioSociSession,
                          stdair::DBRequestStatement_T& ioSelectStatement,
                          stdair::AirlineStruct& ioAirline) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select code, name
         from airlines

      ioSelectStatement =
        (ioSociSession.prepare
         << "select code, name "
         << "from airlines ", soci::into (ioAirline));

      // Execute the SQL query
      ioSelectStatement.execute();
      */

    } catch (std::exception const& lException) {
      STDAIR_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectOnAirlineCodeStatement (stdair::DBSession_T& ioSociSession,
                                       stdair::DBRequestStatement_T& ioSelectStatement,
                                       const stdair::AirlineCode_T& iAirlineCode,
                                       stdair::AirlineStruct& ioAirline) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.code AS code, city_code, xapian_docid, is_airport, is_city,
         is_main, is_commercial, state_code, country_code, region_code,
         continent_code, time_zone_grp, longitude, latitude, language_code,
         classical_name, extended_name, alternate_name1, alternate_name2,
         alternate_name3, alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9, alternate_name10
         from ref_airline_details rpd, ref_airline_names rpn
         where rpd.code = iAirlineCode
           and rpn.code = rpd.code;

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.code AS code, city_code, xapian_docid, is_airport, "
         << "is_city, is_main, is_commercial, state_code, country_code, "
         << "region_code, continent_code, time_zone_grp, longitude, latitude, "
         << "language_code, classical_name, extended_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10 "
         << "from ref_airline_details rpd, ref_airline_names rpn "
         << "where rpd.code = :airline_code "
         << "and rpn.code = rpd.code",
         soci::into (ioAirline), soci::use (iAirlineCode));

      // Execute the SQL query
      ioSelectStatement.execute();
      */

    } catch (std::exception const& lException) {
      STDAIR_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::iterateOnStatement (stdair::DBRequestStatement_T& ioStatement,
                                      stdair::AirlineStruct& ioAirline,
                                      const bool iShouldDoReset) {
    bool hasStillData = false;
  
    try {

      // Reset the list of names of the given Airline object
      if (iShouldDoReset == true) {
        // ioAirline.resetMatrix();
      }

      // Retrieve the next row of Airline object
      hasStillData = ioStatement.fetch();
      
    } catch (std::exception const& lException) {
      STDAIR_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }

    return hasStillData;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::updateAirlineInDB (stdair::DBSession_T& ioSociSession,
                                     const stdair::AirlineStruct& iAirline) {
  
    try {
    
      // Begin a transaction on the database
      ioSociSession.begin();
      
      // Instanciate a SQL statement (no request is performed at that stage)
      std::string lAirlineCode;
      /*
      stdair::DBRequestStatement_T lUpdateStatement =
        (ioSociSession.prepare
         << "update ref_airline_details "
         << "set xapian_docid = :xapian_docid "
         << "where code = :code", soci::use (lDocID), soci::use (lAirlineCode));
      
      // Execute the SQL query
      lDocID = iAirline.getDocID();
      lAirlineCode = iAirline.getAirlineCode();
      lUpdateStatement.execute (true);
      */
      
      // Commit the transaction on the database
      ioSociSession.commit();
        
      // Debug
      // TRADEMGEN_LOG_DEBUG ("[" << lDocID << "] " << iAirline);
      
    } catch (std::exception const& lException) {
      STDAIR_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::retrieveAirline (stdair::DBSession_T& ioSociSession,
                                   const stdair::AirlineCode_T& iAirlineCode,
                                   stdair::AirlineStruct& ioAirline) {
    bool oHasRetrievedAirline = false;
      
    try {

      // Prepare the SQL request corresponding to the select statement
      stdair::DBRequestStatement_T lSelectStatement (ioSociSession);
      DBManager::prepareSelectOnAirlineCodeStatement (ioSociSession,
                                                    lSelectStatement,
                                                    iAirlineCode, ioAirline);
      const bool shouldDoReset = true;
      bool hasStillData = iterateOnStatement (lSelectStatement, ioAirline,
                                              shouldDoReset);
      if (hasStillData == true) {
        oHasRetrievedAirline = true;
      }

      // Sanity check
      const bool shouldNotDoReset = false;
      hasStillData = iterateOnStatement (lSelectStatement, ioAirline,
                                         shouldNotDoReset);
      // Debug
      // STDAIR_LOG_DEBUG ("[" << iDocID << "] " << ioAirline);
      
    } catch (std::exception const& lException) {
      STDAIR_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }

    return oHasRetrievedAirline;
  }

}
