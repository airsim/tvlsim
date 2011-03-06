// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <cmath>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/basic/BasConst_DemandGeneration.hpp>
#include <trademgen/bom/DemandStream.hpp>

namespace TRADEMGEN {

  // ////////////////////////////////////////////////////////////////////
  DemandStream::DemandStream()
    : _key (stdair::DEFAULT_ORIGIN, stdair::DEFAULT_DESTINATION,
            stdair::DEFAULT_FLIGHT_DATE, stdair::DEFAULT_CABIN_CODE),
      _parent (NULL),
      _demandCharacteristics (ArrivalPatternCumulativeDistribution_T(),
                              POSProbabilityMassFunction_T(),
                              ChannelProbabilityMassFunction_T(),
                              TripTypeProbabilityMassFunction_T(),
                              StayDurationProbabilityMassFunction_T(),
                              FrequentFlyerProbabilityMassFunction_T(),
                              PreferredDepartureTimeContinuousDistribution_T(),
                              0.0,
                              ValueOfTimeContinuousDistribution_T()),
      _posProMass (DEFAULT_POS_PROBALILITY_MASS) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  DemandStream::DemandStream (const DemandStream&)
    : _key (stdair::DEFAULT_ORIGIN, stdair::DEFAULT_DESTINATION,
            stdair::DEFAULT_FLIGHT_DATE, stdair::DEFAULT_CABIN_CODE),
      _parent (NULL),
      _demandCharacteristics (ArrivalPatternCumulativeDistribution_T(),
                              POSProbabilityMassFunction_T(),
                              ChannelProbabilityMassFunction_T(),
                              TripTypeProbabilityMassFunction_T(),
                              StayDurationProbabilityMassFunction_T(),
                              FrequentFlyerProbabilityMassFunction_T(),
                              PreferredDepartureTimeContinuousDistribution_T(),
                              0.0,
                              ValueOfTimeContinuousDistribution_T()),
      _posProMass (DEFAULT_POS_PROBALILITY_MASS) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  DemandStream::DemandStream (const Key_T& iKey) : _key (iKey) {
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
  void DemandStream::
  setAll (const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
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
          const POSProbabilityMass_T& iDefaultPOSProbablityMass) {

    setDemandCharacteristics (iArrivalPattern, iPOSProbMass,
                              iChannelProbMass, iTripTypeProbMass,
                              iStayDurationProbMass, iFrequentFlyerProbMass,
                              iPreferredDepartureTimeContinuousDistribution,
                              iMinWTP, iValueOfTimeContinuousDistribution);

    setDemandDistribution (iDemandDistribution);

    setTotalNumberOfRequestsToBeGenerated (0);

    setNumberOfRequestsRandomGeneratorSeed (iNumberOfRequestsSeed);

    setRequestDateTimeRandomGeneratorSeed (iRequestDateTimeSeed);

    setDemandCharacteristicsRandomGeneratorSeed (iDemandCharacteristicsSeed);

    setPOSProbabilityMass (iDefaultPOSProbablityMass);

    //
    init();
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandStream::init() {
    
    // Generate the number of requests
    const stdair::RealNumber_T lMu = _demandDistribution._meanNumberOfRequests;
    const stdair::RealNumber_T lSigma =
      _demandDistribution._stdDevNumberOfRequests;
    
    const stdair::RealNumber_T lRealNumberOfRequestsToBeGenerated =
      _numberOfRequestsRandomGenerator.generateNormal (lMu, lSigma);

    const stdair::NbOfRequests_T lIntegerNumberOfRequestsToBeGenerated = 
      std::floor (lRealNumberOfRequestsToBeGenerated + 0.5);
    
    _totalNumberOfRequestsToBeGenerated = lIntegerNumberOfRequestsToBeGenerated;
  }  

  // ////////////////////////////////////////////////////////////////////
  const bool DemandStream::stillHavingRequestsToBeGenerated() const {
    bool hasStillHavingRequestsToBeGenerated = true;
    
    // Check whether enough requests have already been generated
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      _randomGenerationContext.getNumberOfRequestsGeneratedSoFar();

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
    const stdair::Count_T& lNbOfRequestsGeneratedSoFar =
      _randomGenerationContext.getNumberOfRequestsGeneratedSoFar();

    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      _totalNumberOfRequestsToBeGenerated - lNbOfRequestsGeneratedSoFar;
    assert (lRemainingNumberOfRequestsToBeGenerated > 0);

    // The request date-time is derived from departure date and arrival pattern.
    // Sequential generation
    const stdair::Probability_T& lCumulativeProbabilitySoFar =
      _randomGenerationContext.getCumulativeProbabilitySoFar();

    const stdair::Probability_T& lVariate = _requestDateTimeRandomGenerator();

    //
    const double lRemainingRate =
      1.0 / static_cast<double> (lRemainingNumberOfRequestsToBeGenerated);

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
    _randomGenerationContext.setCumulativeProbabilitySoFar (lCumulativeProbabilityThisRequest);

    //
    incrementGeneratedRequestsCounter();

    // DEBUG
    // STDAIR_LOG_DEBUG (lCumulativeProbabilityThisRequest << "; "
    //                   << lNumberOfDaysBetweenDepartureAndThisRequest);
    
    return oDateTimeThisRequest;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::AirportCode_T DemandStream::generatePOS() {
    stdair::AirportCode_T oPOS;
    
    // Generate a random number between 0 and 1.
    stdair::Probability_T lVariate = _demandCharacteristicsRandomGenerator();
    oPOS = _demandCharacteristics.getPOSValue (lVariate);

    // Check if oPOS is different from 'row'
    if (oPOS == "row") {

      bool lPOSGenerationSucceeded = false;
      while (lPOSGenerationSucceeded == false) {

        lVariate = _demandCharacteristicsRandomGenerator();
        oPOS = _posProMass.getValue (lVariate);
        lPOSGenerationSucceeded = !_demandCharacteristics.checkPOSValue (oPOS);
      }
    }
    
    return oPOS;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::ChannelLabel_T DemandStream::generateChannel() {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator();

    return _demandCharacteristics._channelProbabilityMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::TripType_T DemandStream::generateTripType() {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator(); 

    return _demandCharacteristics._tripTypeProbabilityMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::DayDuration_T DemandStream::generateStayDuration() {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator();    

    return _demandCharacteristics._stayDurationProbabilityMass.getValue (lVariate);
  }
  
  // ////////////////////////////////////////////////////////////////////
  const stdair::FrequentFlyer_T DemandStream::generateFrequentFlyer() {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator();       

    return _demandCharacteristics._frequentFlyerProbabilityMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Duration_T DemandStream::generatePreferredDepartureTime() {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator();     
    const stdair::IntDuration_T lNbOfSeconds = _demandCharacteristics.
      _preferredDepartureTimeCumulativeDistribution.getValue (lVariate);

    const stdair::Duration_T oTime = boost::posix_time::seconds (lNbOfSeconds);

    return oTime;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::WTP_T DemandStream::
  generateWTP (stdair::RandomGeneration& ioGenerator,
               const stdair::Date_T& iDepartureDate,
               const stdair::DateTime_T& iDateTimeThisRequest,
               const stdair::DayDuration_T& iDurationOfStay) {
    const stdair::Date_T lDateThisRequest = iDateTimeThisRequest.date ();
    const stdair::DateOffset_T lAP = iDepartureDate - lDateThisRequest;
    const stdair::DayDuration_T lAPInDays = lAP.days ();
    stdair::RealNumber_T lVariateUnif = 0.90 +  0.2 * ioGenerator();
    const stdair::WTP_T lWTP = 
      lVariateUnif * (365 - lAPInDays) * _demandCharacteristics._minWTP / 36;
    return lWTP;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::PriceValue_T DemandStream::generateValueOfTime() {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      _demandCharacteristicsRandomGenerator();    

    return _demandCharacteristics._valueOfTimeCumulativeDistribution.getValue (lVariate);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandStream::
  generateNextRequest (stdair::RandomGeneration& ioGenerator) {

    // Origin
    const stdair::AirportCode_T& lOrigin = _key.getOrigin();
    // Destination
    const stdair::AirportCode_T& lDestination = _key.getDestination();
    // Preferred departure date
    const stdair::Date_T& lPreferredDepartureDate = 
      _key.getPreferredDepartureDate();
    // Preferred cabin
    const stdair::CabinCode_T& lPreferredCabin = _key.getPreferredCabin();
    // Party size
    const stdair::NbOfSeats_T lPartySize = stdair::DEFAULT_PARTY_SIZE;
    // POS
    const stdair::AirportCode_T lPOS = generatePOS();
    // Time of request.    
    const stdair::DateTime_T lDateTimeThisRequest = generateTimeOfRequest();
    // Booking channel.
    const stdair::ChannelLabel_T lChannelLabel = generateChannel();
    // Trip type.
    const stdair::TripType_T lTripType = generateTripType();
    // Stay duration.
    const stdair::DayDuration_T lStayDuration = generateStayDuration();
    // Frequet flyer type.
    const stdair::FrequentFlyer_T lFrequentFlyer = generateFrequentFlyer();
    // Preferred departure time.
    const stdair::Duration_T lPreferredDepartureTime =
      generatePreferredDepartureTime();
    // Value of time
    const stdair::PriceValue_T lValueOfTime = generateValueOfTime();
    // WTP
    const stdair::WTP_T lWTP = generateWTP (ioGenerator, lPreferredDepartureDate,
                                            lDateTimeThisRequest, lStayDuration);

    // TODO 1: understand why the following form does not work, knowing
    // that:
    // typedef boost::shared_ptr<stdair::BookingRequestStruct> stdair::BookingRequestPtr_T
    // stdair::BookingRequestPtr_T oBookingRequest_ptr =
    //   boost::make_shared<stdair::BookingRequestStruct> ();


    // TODO 2: move the creation of the structure out of the BOM layer
    //  (into the command layer, e.g., within the DemandManager command).
    
    // Create the booking request
    stdair::BookingRequestPtr_T oBookingRequest_ptr =
      stdair::BookingRequestPtr_T (new
                                   stdair::BookingRequestStruct (lOrigin,
                                                                 lDestination,
                                                                 lPOS,
                                                                 lPreferredDepartureDate,
                                                                 lDateTimeThisRequest,
                                                                 lPreferredCabin,
                                                                 lPartySize,
                                                                 lChannelLabel,
                                                                 lTripType,
                                                                 lStayDuration,
                                                                 lFrequentFlyer,
                                                                 lPreferredDepartureTime,
                                                                 lWTP,
                                                                 lValueOfTime));
  
    // DEBUG
    // STDAIR_LOG_DEBUG ("\n[BKG] " << oBookingRequest_ptr->describe());
    
    return oBookingRequest_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandStream::reset() {
    _randomGenerationContext.reset();
    init();
  }  

}
