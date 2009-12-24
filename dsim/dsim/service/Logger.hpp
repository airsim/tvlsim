#ifndef __DSIM_SVC_LOGGER_HPP
#define __DSIM_SVC_LOGGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
// DSIM
#include <dsim/DSIM_Types.hpp>

// /////////////// LOG MACROS /////////////////
#define DSIM_LOG_CORE(iLevel, iToBeLogged) \
  { std::ostringstream ostr; ostr << iToBeLogged; \
    DSIM::Logger::instance().log (iLevel, __LINE__, __FILE__, ostr.str()); }

#define DSIM_LOG_CRITICAL(iToBeLogged) \
  DSIM_LOG_CORE (DSIM::LOG::CRITICAL, iToBeLogged)

#define DSIM_LOG_ERROR(iToBeLogged) \
  DSIM_LOG_CORE (DSIM::LOG::ERROR, iToBeLogged)

#define DSIM_LOG_NOTIFICATION(iToBeLogged) \
  DSIM_LOG_CORE (DSIM::LOG::NOTIFICATION, iToBeLogged)

#define DSIM_LOG_WARNING(iToBeLogged) \
  DSIM_LOG_CORE (DSIM::LOG::WARNING, iToBeLogged)

#define DSIM_LOG_DEBUG(iToBeLogged) \
  DSIM_LOG_CORE (DSIM::LOG::DEBUG, iToBeLogged)

#define DSIM_LOG_VERBOSE(iToBeLogged) \
  DSIM_LOG_CORE (DSIM::LOG::VERBOSE, iToBeLogged)
// /////////// (END OF) LOG MACROS /////////////


namespace DSIM {

  /** Class holding the stream for logs. 
      <br>Note that the error logs are seen as standard output logs, 
      but with a higher level of visibility. */
  class Logger {
    // Friend classes
    friend class FacSupervisor;
  public:
    
    /** Main log entry. */
    template <typename T>
    void log (const LOG::EN_LogLevel iLevel, const int iLineNumber,
              const std::string& iFileName, const T& iToBeLogged) {
      if (iLevel <= _level) {
        assert (_logStream != NULL);
        *_logStream << iFileName << ":" << iLineNumber
                    << ": " << iToBeLogged << std::endl;
      }
    }
    
    /** Get the log level. */
    LOG::EN_LogLevel getLogLevel();
    
    /** get the log stream. */
    std::ostream& getLogStream();
    
    /** Set the logger parameters (level and stream). */
    void setLogParameters (const LOG::EN_LogLevel iLogLevel, 
                           std::ostream& ioLogStream);
    
    /** Returns a current Logger instance.*/
    static Logger& instance();
    
  private:
    /** Default constructors are private so that only the required 
        constructor can be used. */
    Logger ();
    Logger (const Logger&);
    Logger (const LOG::EN_LogLevel iLevel, std::ostream& ioLogStream);
    /** Destructor. */
    ~Logger ();
    
  private:
    /** Log level. */
    LOG::EN_LogLevel _level;
    
    /** Stream dedicated to the logs. */
    std::ostream* _logStream;
    
    /** Instance object.*/
    static Logger* _instance;
  };
  
}
#endif // __DSIM_SVC_LOGGER_HPP
