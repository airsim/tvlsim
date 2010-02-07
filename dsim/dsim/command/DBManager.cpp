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
#include <stdair/dbadaptor/DbaAirline.hpp>
#include <stdair/service/Logger.hpp>
// Dsim
#include <dsim/command/DBManager.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectStatement (stdair::DBSession_T& ioSociSession,
                          stdair::DBRequestStatement_T& ioSelectStatement,
                          stdair::AirlineStruct& ioAirline) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select code, name
         from airlines;
      */

      ioSelectStatement = (ioSociSession.prepare
                           << "select iata_code, name "
                           << "from airlines ", soci::into (ioAirline));

      // Execute the SQL query
      ioSelectStatement.execute();

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
         select code, name
         from airlines
         where code = iAirlineCode;
      */

      ioSelectStatement = (ioSociSession.prepare
                           << "select iata_code, name "
                           << "from airlines "
                           << "where iata_code = :airline_code ",
                           soci::into (ioAirline), soci::use (iAirlineCode));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      STDAIR_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::iterateOnStatement (stdair::DBRequestStatement_T& ioStatement,
                                      stdair::AirlineStruct& ioAirline) {
    bool hasStillData = false;
  
    try {

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

      // Retrieve the airline code
      const std::string& lAirlineCode = iAirline.getAirlineCode();
      
      // Retrieve the airline name
      const std::string& lAirlineName = iAirline.getAirlineName();
      
      // Instanciate a SQL statement (no request is performed at that stage)
      stdair::DBRequestStatement_T lUpdateStatement =
        (ioSociSession.prepare
         << "update airlines "
         << "set name = :name "
         << "where iata_code = :iata_code",
         soci::use (lAirlineName), soci::use (lAirlineCode));
      
      // Execute the SQL query
      lUpdateStatement.execute (true);
      
      // Commit the transaction on the database
      ioSociSession.commit();
        
      // Debug
      // STDAIR_LOG_DEBUG ("[" << lAirlineCode << "] " << iAirline);
      
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
      bool hasStillData = iterateOnStatement (lSelectStatement, ioAirline);
      if (hasStillData == true) {
        oHasRetrievedAirline = true;
      }

      // Sanity check
      const bool shouldNotDoReset = false;
      hasStillData = iterateOnStatement (lSelectStatement, ioAirline);
      
      // Debug
      // STDAIR_LOG_DEBUG ("[" << iDocID << "] " << ioAirline);
      
    } catch (std::exception const& lException) {
      STDAIR_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }

    return oHasRetrievedAirline;
  }

}
