// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Math
#include <cmath>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/bom/DemandStream.hpp>

namespace TRADEMGEN {

  // ////////////////////////////////////////////////////////////////////
  DemandStream::
  DemandStream (const Key_T& iKey,
                const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
                const POSProbabilityMassFunction_T& iPOSProbMass,
                const ChannelProbabilityMassFunction_T& iChannelProbMass,
                const TripTypeProbabilityMassFunction_T& iTripTypeProbMass,
                const StayDurationProbabilityMassFunction_T& iStayDurationProbMass,
                const FrequentFlyerProbabilityMassFunction_T& iFrequentFlyerProbMass,
                const PreferredDepartureTimeContinuousDistribution_T& iPreferredDepartureTimeContinuousDistribution,
                const stdair::WTP_T& iMinWTP,
                const ValueOfTimeContinuousDistribution_T& iValueOfTimeContinuousDistribution,
                const DemandDistribution& iDemandDistribution,
                const stdair::RandomSeed_T& iNumberOfRequestsSeed,
                const stdair::RandomSeed_T& iRequestDateTimeSeed,
                const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
                stdair::UniformGenerator_T& ioSharedGenerator,
                const POSProbabilityMass_T& iDefaultPOSProbablityMass)
    : _key (iKey),
      _demandCharacteristics (iArrivalPattern, iPOSProbMass,
                              iChannelProbMass, iTripTypeProbMass,
                              iStayDurationProbMass, iFrequentFlyerProbMass,
                              iPreferredDepartureTimeContinuousDistribution,
                              iMinWTP, iValueOfTimeContinuousDistribution),
      _demandDistribution (iDemandDistribution),
      _totalNumberOfRequestsToBeGenerated (0),
      _numberOfRequestsRandomGenerator (iNumberOfRequestsSeed),
      _requestDateTimeRandomGenerator (iRequestDateTimeSeed),
      _demandCharacteristicsRandomGenerator (iDemandCharacteristicsSeed),
      _uniformGenerator (ioSharedGenerator),
      _posProMass (iDefaultPOSProbablityMass) {
    init();
  }

  // ////////////////////////////////////////////////////////////////////
  DemandStream::~DemandStream() {
  }

  // ////////////////////////////////////////////////////////////////////
  std::string DemandStream::toString() const {
    std::ostringstream oStr;
    oStr << _key.toString();
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandStream::init() {
    
    // Generate the number of requests
    const stdair::RealNumber_T lMu = _demandDistribution._meanNumberOfRequests;
    const stdair::RealNumber_T lSigma =
      _demandDistribution._stdDevNumberOfRequests;
    
    const stdair::RealNumber_T lRealNumberOfRequestsToBeGenerated =
      _numberOfRequestsRandomGenerator.generateNormal (lMu, lSigma);

    stdair::NbOfRequests_T lIntegerNumberOfRequestsToBeGenerated = 0;
    if (lRealNumberOfRequestsToBeGenerated < 0.5) {
    	lIntegerNumberOfRequestsToBeGenerated = 0;
        
    } else {
      lIntegerNumberOfRequestsToBeGenerated =
        static_cast<stdair::NbOfRequests_T> (lRealNumberOfRequestsToBeGenerated + 0.5);
    }
    
    _totalNumberOfRequestsToBeGenerated = lIntegerNumberOfRequestsToBeGenerated;
  }  

  // ////////////////////////////////////////////////////////////////////
  const bool DemandStream::stillHavingRequestsToBeGenerated() const {
    bool hasStillHavingRequestsToBeGenerated = true;
    
    // Check whether enough requests have already been generated
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      _randomGenerationContext._numberOfRequestsGeneratedSoFar;

    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      _totalNumberOfRequestsToBeGenerated - lNbOfRequestsGeneratedSoFar;

    if (lRemainingNumberOfRequestsToBeGenerated <= 0) {
      hasStillHavingRequestsToBeGenerated = false;
    }
    
    return hasStillHavingRequestsToBeGenerated;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::DateTime_T DemandStream::generateTimeOfRequest() {
    
    // Assert that there are requests to be generated.
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      _randomGenerationContext._numberOfRequestsGeneratedSoFar;

    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      _totalNumberOfRequestsToBeGenerated - lNbOfRequestsGeneratedSoFar;
    assert (lRemainingNumberOfRequestsToBeGenerated > 0);

    // Request datetime, determined from departure date and arrival pattern
    // Sequential generation
    const stdair::Probability_T lCumulativeProbabilitySoFar =
      _randomGenerationContext._cumulativeProbabilitySoFar;

    const stdair::Probability_T lVariate =
      _requestDateTimeRandomGenerator.generateUniform01();

    //
    const float lRemainingRate =
      1.0 / static_cast<float> (lRemainingNumberOfRequestsToBeGenerated);

    //
    const stdair::Probability_T lComplementOfCumulativeProbabilitySoFar =
      1.0 - lCumulativeProbabilitySoFar;
      
    //
    const stdair::Probability_T lCumulativeProbabilityThisRequest =
      1.0 -
      lComplementOfCumulativeProbabilitySoFar * std::pow (1 - lVariate,
                                                          lRemainingRate);

    //
    const stdair::FloatDuration_T lNumberOfDaysBetweenDepartureAndThisRequest =
      _demandCharacteristics._arrivalPattern.getValue (lCumulativeProbabilityThisRequest);

    // Convert the number of days in number of seconds + number of milliseconds
    const stdair::FloatDuration_T lNumberOfSeconds =
      lNumberOfDaysBetweenDepartureAndThisRequest * stdair::SECONDS_IN_ONE_DAY;

    //
    const stdair::IntDuration_T lIntNumberOfSeconds =
      std::floor (lNumberOfSeconds);

    //
    const stdair::FloatDuration_T lNumberOfMilliseconds =
      (lNumberOfSeconds - lIntNumberOfSeconds)
      * stdair::MILLISECONDS_IN_ONE_SECOND;

    // +1 is a trick to ensure that the next Event is strictly later
    // than the current one
    const stdair::IntDuration_T lIntNumberOfMilliseconds =
      std::floor (lNumberOfMilliseconds) + 1;

    const stdair::Duration_T lDifferenceBetweenDepartureAndThisRequest =
      boost::posix_time::seconds (lIntNumberOfSeconds)
      + boost::posix_time::millisec (lIntNumberOfMilliseconds);

    const stdair::Time_T lHardcodedReferenceDepartureTime =
      boost::posix_time::hours (8);
    
    const stdair::DateTime_T lDepartureDateTime =
      boost::posix_time::ptime (_key.getPreferredDepartureDate(),
                                lHardcodedReferenceDepartureTime);

    const stdair::DateTime_T oDateTimeThisRequest =
      lDepartureDateTime + lDifferenceBetweenDepartureAndThisRequest;
    
    // Update random generation context
    _randomGenerationContext._cumulativeProbabilitySoFar =
      lCumulativeProbabilityThisRequest;

    //
    incrementGeneratedRequestsCounter ();

    // DEBUG
    // STDAIR_LOG_DEBUG (lCumulativeProbabilityThisRequest << "; "
    //                   << lNumberOfDaysBetweenDepartureAndThisRequest);
    
    return oDateTimeThisRequest;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::AirportCode_T DemandStream::generatePOS () {
    stdair::AirportCode_T oPOS;
    
    // Generate a random number between 0 and 1.
    stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator.generateUniform01();
    oPOS = _demandCharacteristics._posProbabilityMass.getValue (lVariate);

    // Check if oPOS is different from 'row'
    if (oPOS == "row") {
      bool lPOSGenerationSucceeded = false;
      while (lPOSGenerationSucceeded == false) {
        lVariate =_demandCharacteristicsRandomGenerator.generateUniform01();
        oPOS = _posProMass.getValue (lVariate);
        lPOSGenerationSucceeded = 
          1-_demandCharacteristics._posProbabilityMass.checkValue (oPOS);
      }
    }
    
    return oPOS;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::ChannelLabel_T DemandStream::generateChannel () {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator.generateUniform01();

    return _demandCharacteristics._channelProbabilityMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::TripType_T DemandStream::generateTripType () {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator.generateUniform01(); 

    return _demandCharacteristics._tripTypeProbabilityMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::DayDuration_T DemandStream::generateStayDuration () {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator.generateUniform01();    

    return _demandCharacteristics._stayDurationProbabilityMass.getValue (lVariate);
  }
  
  // ////////////////////////////////////////////////////////////////////
  const stdair::FrequentFlyer_T DemandStream::generateFrequentFlyer () {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator.generateUniform01();       

    return _demandCharacteristics._frequentFlyerProbabilityMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Duration_T DemandStream::generatePreferredDepartureTime () {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator.generateUniform01();     
    const stdair::IntDuration_T lNbOfSeconds = _demandCharacteristics.
      _preferredDepartureTimeCumulativeDistribution.getValue (lVariate);

    const stdair::Duration_T oTime = boost::posix_time::seconds (lNbOfSeconds);

    return oTime;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::WTP_T DemandStream::
  generateWTP (const stdair::Date_T& iDepartureDate,
               const stdair::DateTime_T& iDateTimeThisRequest,
               const stdair::DayDuration_T& iDurationOfStay) {
    const stdair::Date_T lDateThisRequest = iDateTimeThisRequest.date ();
    const stdair::DateOffset_T lAP = iDepartureDate - lDateThisRequest;
    const stdair::DayDuration_T lAPInDays = lAP.days ();
    stdair::RealNumber_T lVariateUnif = 0.90 + _uniformGenerator() * 0.2;
    const stdair::WTP_T lWTP = 
      lVariateUnif * (365 - lAPInDays) * _demandCharacteristics._minWTP / 36;
    return lWTP;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::PriceValue_T DemandStream::generateValueOfTime () {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator.generateUniform01();    

    return _demandCharacteristics._valueOfTimeCumulativeDistribution.getValue (lVariate);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandStream::generateNextRequest () {
    // Origin
    const stdair::AirportCode_T& lOrigin = _key.getOrigin();
    // Destination
    const stdair::AirportCode_T& lDestination = _key.getDestination();
    // Preferred departure date
    const stdair::Date_T& lPreferredDepartureDate = 
      _key.getPreferredDepartureDate();
    // Preferred cabin
    const stdair::CabinCode_T& lPreferredCabin = _key.getPreferredCabin();
    // POS
    const stdair::AirportCode_T lPOS = generatePOS ();
    // Time of request.    
    const stdair::DateTime_T lDateTimeThisRequest =  generateTimeOfRequest ();
    // Booking channel.
    const stdair::ChannelLabel_T lChannelLabel = generateChannel ();
    // Trip type.
    const stdair::TripType_T lTripType = generateTripType ();
    // Stay duration.
    const stdair::DayDuration_T lStayDuration = generateStayDuration ();
    // Frequet flyer type.
    const stdair::FrequentFlyer_T lFrequentFlyer = generateFrequentFlyer ();
    // Preferred departure time.
    const stdair::Duration_T lPreferredDepartureTime =
      generatePreferredDepartureTime ();
    // Value of time
    const stdair::PriceValue_T lValueOfTime = generateValueOfTime ();
    // WTP
    const stdair::WTP_T lWTP = generateWTP (lPreferredDepartureDate,
                                            lDateTimeThisRequest,
                                            lStayDuration);
    
    // Create the booking request with a hardcoded party size.
    stdair::BookingRequestPtr_T oBookingRequest_ptr = stdair::BookingRequestPtr_T
      (new stdair::BookingRequestStruct (lOrigin, lDestination, lPOS,
                                         lPreferredDepartureDate,
                                         lDateTimeThisRequest,
                                         lPreferredCabin,
                                         stdair::DEFAULT_PARTY_SIZE,
                                         lChannelLabel, lTripType, lStayDuration,
                                         lFrequentFlyer, lPreferredDepartureTime,
                                         lWTP, lValueOfTime));
    // DEBUG
    // STDAIR_LOG_DEBUG ("\n[BKG] " << oBookingRequest_ptr->describe());
    
    return oBookingRequest_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandStream::reset () {
    init ();
    _randomGenerationContext.reset();
  }  

}
