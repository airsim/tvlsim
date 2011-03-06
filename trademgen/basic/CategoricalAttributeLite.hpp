#ifndef __TRADEMGEN_BAS_CATEGORICALATTRIBUTELITE_HPP
#define __TRADEMGEN_BAS_CATEGORICALATTRIBUTELITE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include <map>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Exceptions.hpp>
#include <trademgen/basic/DictionaryManager.hpp>

namespace TRADEMGEN {

  /**
   * @brief Class modeling the distribution of values that can be
   * taken by a categorical attribute.
   */
  template <typename T>
  struct CategoricalAttributeLite {
    public:
    // ///////////// Type definitions //////////////
    /**
     * Type for the probability mass function.
     */
    typedef std::map<T, stdair::Probability_T> ProbabilityMassFunction_T;
    

  public:
    // /////////////// Business Methods //////////
    /**
     * Get value from inverse cumulative distribution.
     */
    const T& getValue (const stdair::Probability_T& iCumulativeProbability) const {
      const DictionaryKey_T& lKey =
        DictionaryManager::valueToKey (iCumulativeProbability);

      for (unsigned int idx = 0; idx < _size; ++idx) {
        if (_cumulativeDistribution.at(idx) >= lKey) {
          const T& oValue = _valueArray.at(idx);
          return oValue;
        }
      }

      std::ostringstream oStr;
      oStr << "The following cumulative probability is out of range: "
           << iCumulativeProbability << displayProbabilityMass();
      throw IndexOutOfRangeException (oStr.str());
    }

    /**
     * Check if a value belongs to the value list.
     */
    bool checkValue (const T& iValue) const {
      for (unsigned int idx = 0; idx < _size; ++idx) {
        if (_valueArray.at(idx) == iValue) {
          return true;
        }
      }
      return false;
    }
    

  public:
    // ////////////// Display Support Methods //////////
    /**
     * Display probability mass function.
     */
    const std::string displayProbabilityMass() const {
      std::ostringstream oStr;

      for (unsigned int idx = 0; idx < _size; ++idx) {
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << _valueArray.at(idx) << ":"
             << DictionaryManager::keyToValue (_cumulativeDistribution[idx]);
      }
      return oStr.str();
    }
        

  public:
    // ////////// Constructors and destructors /////////
    /**
     * Main constructor.
     */
    CategoricalAttributeLite (const ProbabilityMassFunction_T& iValueMap)
      : _size (iValueMap.size()) {
      init (iValueMap);
    }

    /**
     * Default constructor.
     */
    CategoricalAttributeLite() : _size(1) {
    }

    /**
     * Copy constructor.
     */
    CategoricalAttributeLite (const CategoricalAttributeLite& iCAL)
      : _size (iCAL._size),
        _cumulativeDistribution (iCAL._cumulativeDistribution),
        _valueArray (iCAL._valueArray) {
    }

    /**
     * Copy operator.
     */
    CategoricalAttributeLite& operator= (const CategoricalAttributeLite& iCAL) {
      _size = iCAL._size;
      _cumulativeDistribution = iCAL._cumulativeDistribution;
      _valueArray = iCAL._valueArray;
      return *this;
    }

    /**
     * Destructor.
     */
    virtual ~CategoricalAttributeLite() {
    }


  private:
    /**
     * Initialise the two arrays from the given map.
     */
    void init (const ProbabilityMassFunction_T& iValueMap) {
      
      const unsigned int lSize = iValueMap.size();
      _cumulativeDistribution.reserve (lSize);
      _valueArray.reserve (lSize);

      stdair::Probability_T cumulative_probability_so_far = 0.0;

      // Browse the map to retrieve the values and to build the
      // cumulative probabilities.
      for (typename ProbabilityMassFunction_T::const_iterator
             itProbabilityMassFunction = iValueMap.begin();
           itProbabilityMassFunction != iValueMap.end();
           ++itProbabilityMassFunction) {
        
        stdair::Probability_T attribute_probability_mass =
          itProbabilityMassFunction->second;

        if (attribute_probability_mass > 0) {
          const T& attribute_value = itProbabilityMassFunction->first;
          cumulative_probability_so_far += attribute_probability_mass;

          const DictionaryKey_T& lKey =
            DictionaryManager::valueToKey (cumulative_probability_so_far);

          // Build the two arrays.
          _cumulativeDistribution.push_back (lKey);
          _valueArray.push_back (attribute_value);
        }
      }
    }
  
  private:
    // ////////// Attributes //////////
    /**
     * Size of the two arrays.
     */
    unsigned int _size;
    
    /**
     * Cumulative dictionary-coded distribution.
     */
    std::vector<DictionaryKey_T> _cumulativeDistribution;

    /**
       The corresponding values.
    */
    std::vector<T> _valueArray;
  };
}
#endif // __TRADEMGEN_BAS_CATEGORICALATTRIBUTELITE_HPP
