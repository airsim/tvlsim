/* Log library: contains some useful macros for logging */

#include <sstream>
#include <fstream>
#include <string>
#include <map>

/* logFile: file contains log */
/* iToBeLogged: message needs to be looged */
/* iLevel: log level(ex: CRITICAL, NOTIFICATION,...)*/
// /////////////// LOG MACROS /////////////////


#define LOG_CORE(logFile,iLevel, iToBeLogged) \
  { std::ostringstream ostr; ostr << iToBeLogged; \
    log (logFile,iLevel, __LINE__, __FILE__, ostr.str()); }

#define LOG_CRITICAL(logFile,iToBeLogged) \
  LOG_CORE (logFile,CRITICAL, iToBeLogged)

#define LOG_ERROR(logFile,iToBeLogged) \
  LOG_CORE (logFile,ERROR, iToBeLogged)

#define LOG_NOTIFICATION(logFile,iToBeLogged) \
  LOG_CORE (logFile,NOTIFICATION, iToBeLogged)

#define LOG_WARNING(logFile,iToBeLogged) \
  LOG_CORE (logFile,WARNING, iToBeLogged)

#define LOG_DEBUG(logFile,iToBeLogged) \
  LOG_CORE (logFile,DEBUG, iToBeLogged)

#define LOG_VERBOSE(logFile,iToBeLogged) \
  LOG_CORE (logFile,VERBOSE, iToBeLogged)
// /////////// (END OF) LOG MACROS /////////////


typedef enum {
	CRITICAL = 0,
	ERROR,
	NOTIFICATION,
	WARNING,
	DEBUG,
	VERBOSE,
	LAST_VALUE
} EN_LogLevel;

	
/** Main log entry. */
void log (std::string logFile,const EN_LogLevel iLevel, const int iLineNumber,
			const std::string& iFileName, std::string iToBeLogged) {
	using namespace std;
		
	std::map<int,string> logTable;
	
	logTable[CRITICAL] = "CRITICAL";
	logTable[ERROR] = "ERROR";
	logTable[NOTIFICATION]="NOTIFICATION";
	logTable[WARNING]="WARNING";
	logTable[DEBUG]="DEBUG";
	logTable[VERBOSE]="VERBOSE";
	logTable[LAST_VALUE]="LAST_VALUE";

	ofstream out;
	out.open(logFile.c_str(), ios_base::app);
	out << "[" << logTable[iLevel] << "]" << iFileName << ":"
			<< iLineNumber << ": " << iToBeLogged << std::endl;
	out.close();
}

/*Testing the possibility to use LOG_DEBUG*/
void log_testing(){
	 LOG_DEBUG("test.log","hello!");
	
}


