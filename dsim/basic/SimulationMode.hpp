#ifndef __DSIM_BAS_SIMULATIONMODE_HPP
#define __DSIM_BAS_SIMULATIONMODE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace DSIM {

  /**
   * @brief Enumeration of json commands.
   */
  struct SimulationMode : public stdair::StructAbstract {
  public:
    typedef enum { 
      RUNNING = 0,
      BREAK,
      DONE,	
      LAST_VALUE
    } EN_SimulationMode;

    /**
     * Get the label as a string (e.g., "running", "break" or "done").
     */
    static const std::string& getLabel (const EN_SimulationMode&);

    /**
     * Get the mode value from parsing a single char (e.g., 'r', 'b', 'd').
     */
    static EN_SimulationMode getMode (const char);

    /**
     * Get the label as a single char (e.g., 'r', 'b', 'd').
     */
    static char getModeLabel (const EN_SimulationMode&);

    /**
     * Get the label as a string of a single char (e.g., 'r', 'b', 'd').
     */
    static std::string getModeLabelAsString (const EN_SimulationMode&);

    /**
     * List the labels.
     */
    static std::string describeLabels();

    /**
     * Get the enumerated value.
     */
    EN_SimulationMode getMode() const;

    /**
     * Get the enumerated value as a char (e.g., 'r', 'b', 'd').
     */
    char getModeAsChar() const;

    /**
     * Get the enumerated value as a short string (e.g., "r", "b", "d").
     */
    std::string getModeAsString() const;
    
    /**
     * Give a description of the structure (e.g., "running", "break" or "done").
     */
    const std::string describe() const;
 
  public:
    /**
     * Setters
     */
    void setMode (const EN_SimulationMode& iEN_SimulationMode) {
      _mode = iEN_SimulationMode;
    }

  public:
    /**
     * Comparison operator.
     */
    bool operator== (const EN_SimulationMode&) const;
    
  public:
    /**
     * Main Constructor.
     */
    SimulationMode (const EN_SimulationMode&);
    
    /**
     * Constructor.
     */
    SimulationMode (const char);
    
    /**
     * Alternative constructor.
     */
    SimulationMode (const std::string&);
    
    /**
     * Default copy constructor.
     */
    SimulationMode (const SimulationMode&);

  private:
    /**
     * Default constructor.
     */
    SimulationMode();

    
  private:
    /**
     * String version of the enumeration.
     */    
    static const std::string _labels[LAST_VALUE];
    
    /**
     * Char version of the enumeration.
     */    
    static const char _modeLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /**
     * Simulation mode.
     */
    EN_SimulationMode _mode;
  };

}
#endif // __DSIM_BAS_SIMULATIONMODE_HPP
