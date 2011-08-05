// STL
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
// Boost String
#include <boost/python.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>

namespace TRADEMGEN {

  struct Trademgener {
  public:
    /** Wrapper around the travel demand generation use case. */
    std::string trademgen (const std::string& iQuery) {
      std::ostringstream oStream;

      // Sanity check
      if (_logOutputStream == NULL) {
        oStream << "The log filepath is not valid." << std::endl;
        return oStream.str();
      }
      assert (_logOutputStream != NULL);
      
      try {

        // DEBUG
        *_logOutputStream << "Python search for '" << iQuery << "'"
                          << std::endl;
      
        if (_trademgenService == NULL) {
          oStream << "The Trademgen service has not been initialised, "
                  << "i.e., the init() method has not been called "
                  << "correctly on the Trademgener object. Please "
                  << "check that all the parameters are not empty and "
                  << "point to actual files.";
          *_logOutputStream << oStream.str();
          return oStream.str();
        }
        assert (_trademgenService != NULL);
        
        // Do the trademgen
        _trademgenService->displayAirlineListFromDB();

        // DEBUG
        *_logOutputStream << "Python search for '" << iQuery
                          << "' returned '" << std::endl;

        // DEBUG
        *_logOutputStream << "TraDemGen output: "
                          << oStream.str() << std::endl;

      } catch (const stdair::RootException& eTrademgenError) {
        *_logOutputStream << "TraDemGen error: "  << eTrademgenError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      return oStream.str();
    }

  public:
    /** Default constructor. */
    Trademgener() : _trademgenService (NULL), _logOutputStream (NULL) {
    }
    
    /** Default copy constructor. */
    Trademgener (const Trademgener& iTrademgener)
      : _trademgenService (iTrademgener._trademgenService),
        _logOutputStream (iTrademgener._logOutputStream) {
    }

    /** Default constructor. */
    ~Trademgener() {
      _trademgenService = NULL;
      _logOutputStream = NULL;
    }
    
    /** Wrapper around the search use case. */
    bool init (const std::string& iLogFilepath,
               const stdair::Filename_T& iDemandInputFilename,
               const std::string& iDBUser, const std::string& iDBPasswd,
               const std::string& iDBHost, const std::string& iDBPort,
               const std::string& iDBDBName) {
      bool isEverythingOK = true;

      try {
        
        // Check that the file path given as input corresponds to an actual file
        const bool isWriteable = (iLogFilepath.empty() == false);
        // stdair::BasFileMgr::isWriteable (iLogFilepath);
        if (isWriteable == false) {
          isEverythingOK = false;
          return isEverythingOK;
        }
        
        // Set the log parameters
        _logOutputStream = new std::ofstream;
        assert (_logOutputStream != NULL);
        
        // Open and clean the log outputfile
        _logOutputStream->open (iLogFilepath.c_str());
        _logOutputStream->clear();
        
        // DEBUG
        *_logOutputStream << "Python wrapper initialisation" << std::endl;
        const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG,
                                               *_logOutputStream);
        
        // Initialise the context
        stdair::BasDBParams lDBParams (iDBUser, iDBPasswd, iDBHost, iDBPort,
                                       iDBDBName);
        _trademgenService = new TRADEMGEN_Service (lLogParams, lDBParams);
        assert (_trademgenService != NULL);

        // Create the DemandStream objects, and insert them within the BOM tree
        _trademgenService->parseAndLoad (iDemandInputFilename);

        // DEBUG
        *_logOutputStream << "Python wrapper initialised" << std::endl;
        
      } catch (const stdair::RootException& eTrademgenError) {
        *_logOutputStream << "Trademgen error: "  << eTrademgenError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }
      
      return isEverythingOK;
    }

  private:
    /** Handle on the Trademgen services (API). */
    TRADEMGEN_Service* _trademgenService;
    std::ofstream* _logOutputStream;
  };

}

// /////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(libpytrademgen) {
  boost::python::class_<TRADEMGEN::Trademgener> ("Trademgener")
    .def ("trademgen", &TRADEMGEN::Trademgener::trademgen)
    .def ("init", &TRADEMGEN::Trademgener::init);
}
