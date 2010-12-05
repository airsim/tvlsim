// STL
#include <fstream>
#include <set>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// Boost Spirit (Parsing)
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>


// ////////////////////////////////////////////////////////////////////////////
//
//  Namespace aliases and type definitions
//
///////////////////////////////////////////////////////////////////////////////
namespace bp  = boost::phoenix;
namespace bsa = boost::spirit::ascii;
namespace bsq = boost::spirit::qi;


/** LegCabin-Details. */
struct Cabin_T {
  // Attributes
  std::string _cabinCode;
  double _capacity;

  void display() const {
    std::cout << "        " << _cabinCode << " " << _capacity << ", ";
  }
};

/** List of Cabin-Detail strucutres. */
typedef std::vector<Cabin_T> CabinList_T;

/** Leg. */
struct Leg_T {
  // Attributes
  std::string _boardPoint;
  boost::posix_time::time_duration _boardTime;
  boost::gregorian::date_duration _boardDateOffSet;
  std::string _offPoint;
  boost::posix_time::time_duration _offTime;
  boost::gregorian::date_duration _offDateOffSet;
  boost::posix_time::time_duration _elapsed;
  CabinList_T _cabinList;

  /** Constructor. */
  Leg_T () : _boardDateOffSet (0), _offDateOffSet (0) {}
  
  void display() const {
    std::cout << "    " << _boardPoint << " / "
              << boost::posix_time::to_simple_string (_boardTime)
              << " -- " << _offPoint << " / "
              << boost::posix_time::to_simple_string (_offTime)
              << " --> " << boost::posix_time::to_simple_string (_elapsed)
              << std::endl;
    for (CabinList_T::const_iterator itCabin = _cabinList.begin();
	 itCabin != _cabinList.end(); itCabin++) {
      const Cabin_T& lCabin = *itCabin;
      lCabin.display();
    }
    std::cout << std::endl;
  }
};

/** List of Leg strucutres. */
typedef std::vector<Leg_T> LegList_T;

/** SegmentCabin-Details. */
struct SegmentCabin_T {
  // Attributes
  std::string _cabinCode;
  std::string _classes;

  void display() const {
    std::cout << "        " << _cabinCode << " " << _classes << ", ";
  }
};

/** List of SegmentCabin-Detail strucutres. */
typedef std::vector<SegmentCabin_T> SegmentCabinList_T;

/** Segment. */
struct Segment_T {
  // Attributes
  std::string _boardPoint;
  boost::posix_time::time_duration _boardTime;
  boost::gregorian::date_duration _boardDateOffSet;
  std::string _offPoint;
  boost::posix_time::time_duration _offTime;
  boost::gregorian::date_duration _offDateOffSet;
  boost::posix_time::time_duration _elapsed;
  SegmentCabinList_T _cabinList;
  
  /** Constructor. */
  Segment_T () : _boardDateOffSet (0), _offDateOffSet (0) {}
  
  void display() const {
    std::cout << "    " << _boardPoint << " / "
              << boost::posix_time::to_simple_string (_boardTime)
              << " -- " << _offPoint << " / "
              << boost::posix_time::to_simple_string (_offTime)
              << " --> " << boost::posix_time::to_simple_string (_elapsed)
              << std::endl;
    for (SegmentCabinList_T::const_iterator itCabin = _cabinList.begin();
	 itCabin != _cabinList.end(); itCabin++) {
      const SegmentCabin_T& lCabin = *itCabin;
      lCabin.display();
    }
    std::cout << std::endl;
  }
};

/** List of Segment strucutres. */
typedef std::vector<Segment_T> SegmentList_T;

/** Flight-Period. */
struct FlightPeriod_T {

  // Attributes
  std::string _airlineCode;
  unsigned int _flightNumber;
  boost::gregorian::date _dateRangeStart;
  boost::gregorian::date _dateRangeEnd;
  std::string _dow;
  LegList_T _legList;
  SegmentList_T _segmentList;
  
  /** Constructor. */
  FlightPeriod_T () : _legAlreadyDefined (false), _itSeconds (0) {}

  /** Set the date from the staging details. */
  boost::gregorian::date getDate() const {
    return boost::gregorian::date (_itYear, _itMonth, _itDay);
  }

  /** Set the time from the staging details. */
  boost::posix_time::time_duration getTime() const {
    return boost::posix_time::hours (_itHours)
      + boost::posix_time::minutes (_itMinutes)
      + boost::posix_time::seconds (_itSeconds);
  }
  
  void display() const {
    std::cout << _airlineCode << _flightNumber
              << ", " << boost::gregorian::to_simple_string (_dateRangeStart)
              << " - " << boost::gregorian::to_simple_string (_dateRangeEnd)
              << " - " << _dow
              << std::endl;
    
    for (LegList_T::const_iterator itLeg = _legList.begin();
	 itLeg != _legList.end(); itLeg++) {
      const Leg_T& lLeg = *itLeg;
      lLeg.display();
    }

    for (SegmentList_T::const_iterator itSegment = _segmentList.begin();
	 itSegment != _segmentList.end(); itSegment++) {
      const Segment_T& lSegment = *itSegment;
      lSegment.display();
    }
    
    //std::cout << "[Debug] - Staging Leg: ";
    //_itLeg.display();
    //std::cout << "[Debug] - Staging Cabin: ";
    //_itCabin.display();
    //std::cout << "[Debug] - Staging Segment: ";
    //_itSegment.display();
  }

  /** Add the given airport to the internal lists (if not already existing). */
  void addAirport (const std::string& iAirport) {
    std::set<std::string>::const_iterator itAirport =
      _airportList.find (iAirport);
    if (itAirport == _airportList.end()) {
      // Add the airport code to the airport set
      const bool insertSuccessful = _airportList.insert (iAirport).second;
      
      if (insertSuccessful == false) {
        // TODO: throw an exception
      }
      // Add the airport code to the airport vector
      _airportOrderedList.push_back (iAirport);
    }
  }
  
  /** Build the routing (segments). */
  void buildSegments () {
    // The list of airports encompasses all the airports on which
    // the flight takes off or lands. Moreover, that list is
    // time-ordered: the first airport is the initial departure of
    // the flight, and the last airport is the eventual point of
    // rest of the flight.
    // Be l the size of the ordered list of airports.
    // We want to generate all the segment combinations from the legs
    // and, hence, from all the possible (time-ordered) airport pairs.
    // Thus, we both iterator on i=0...l-1 and j=i+1...l
    assert (_airportOrderedList.size() >= 2);

    _segmentList.clear();
    for (std::vector<std::string>::const_iterator itAirport_i =
           _airportOrderedList.begin();
         itAirport_i != _airportOrderedList.end()-1; ++itAirport_i) {
      for (std::vector<std::string>::const_iterator itAirport_j =
             itAirport_i + 1;
           itAirport_j != _airportOrderedList.end(); ++itAirport_j) {
        Segment_T lSegment;
        lSegment._boardPoint = *itAirport_i;
        lSegment._offPoint = *itAirport_j;
        
        _segmentList.push_back (lSegment);
      }
    }

    // Clear the lists of airports, so that it is ready for the next flight
    _airportList.clear();
    _airportOrderedList.clear();
  }
  
  /** Add, to the Segment whose key corresponds to the
      given (board point, off point) pair, the specific segment cabin
      details (mainly, the list of the class codes).
      <br>Note that the Segment structure is retrieved from the internal
      list, already filled by a previous step (the buildSegments()
      method). */
  void addSegmentCabin (const Segment_T& iSegment,
                        const SegmentCabin_T& iCabin) {
    // Retrieve the Segment structure corresponding to the (board, off) point
    // pair.
    SegmentList_T::iterator itSegment = _segmentList.begin();
    for ( ; itSegment != _segmentList.end(); ++itSegment) {
      const Segment_T& lSegment = *itSegment;
      
      const std::string& lBoardPoint = iSegment._boardPoint;
      const std::string& lOffPoint = iSegment._offPoint;
      if (lSegment._boardPoint == lBoardPoint
          && lSegment._offPoint == lOffPoint) {
        break;
      }
    }
    
    // If the segment key (airport pair) given in the schedule input file
    // does not correspond to the leg (board, off) points, throw an exception
    // so that the user knows the schedule input file is corrupted.
    if (itSegment == _segmentList.end()) {
      std::cerr << "Within the schedule input file, there is a flight for which the airports of segments and those of the legs do not correspond.";
      throw std::exception();
    }
    
    // Add the Cabin structure to the Segment Cabin structure.
    assert (itSegment != _segmentList.end());
    Segment_T& lSegment = *itSegment;
    lSegment._cabinList.push_back (iCabin);
  }
      
  /** Add, to all the Segment, the general segment cabin details
      (mainly, the list of the class codes).
      <br>Note that the Segment structures are stored within the internal
      list, already filled by a previous step (the buildSegments()
      method). */
  void addSegmentCabin (const SegmentCabin_T& iCabin) {
    // Iterate on all the Segment (as they get the same cabin definitions)
    for (SegmentList_T::iterator itSegment = _segmentList.begin();
         itSegment != _segmentList.end(); ++itSegment) {
      Segment_T& lSegment = *itSegment;
      lSegment._cabinList.push_back (iCabin);
    }
  }
  
  /** Staging Leg (resp. Cabin) structure, gathering the result of the iteration
      on one leg (resp. cabin). */
  bool _legAlreadyDefined;
  Leg_T _itLeg;
  Cabin_T _itCabin;

  /** Staging Date. */
  unsigned int _itYear;
  unsigned int _itMonth;
  unsigned int _itDay;

  /** Staging Time. */
  long _itHours;
  long _itMinutes;
  long _itSeconds;
  int _dateOffSet;

  /** Staging Airport List (helper to derive the list of Segment
      structures). */
  std::set<std::string> _airportList;
  std::vector<std::string> _airportOrderedList;

  /** Staging Segment-related attributes. */
  bool _areSegmentDefinitionsSpecific;
  Segment_T _itSegment;
  SegmentCabin_T _itSegmentCabin;
 
};

///////////////////////////////////////////////////////////////////////////////
//
//  Semantic actions
//
///////////////////////////////////////////////////////////////////////////////

/** Store the parsed airline code. */
struct store_airline_code {
  store_airline_code (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (std::vector<char> iChar,
                   bsq::unused_type,
                   bsq::unused_type) const { 
    std::string lAirlineCode (iChar.begin(), iChar.end());
    _flightPeriod._airlineCode = lAirlineCode;
    // std::cout << "Airline code: " << lAirlineCode << std::endl;

    // As that's the beginning of a new flight, the list of legs must be reset
      _flightPeriod._legList.clear();
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the parsed flight number. */
struct store_flight_number {
  store_flight_number (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
  
  void operator() (unsigned int iNumber,
                   bsq::unused_type,
                   bsq::unused_type) const { 
    _flightPeriod._flightNumber = iNumber;
    //std::cout << "Flight number: " << iNumber << std::endl;
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the start of the date range. */
struct store_date_range_start {
  store_date_range_start (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (bsq::unused_type,
                   bsq::unused_type,
                   bsq::unused_type) const { 
    _flightPeriod._dateRangeStart = _flightPeriod.getDate();
    //std::cout << "Date Range Start: "
    // << _flightPeriod._dateRangeStart << std::endl;
      
    // Reset the number of seconds
    _flightPeriod._itSeconds = 0;
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the end of the date range. */
struct store_date_range_end {
  store_date_range_end (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
  
  void operator() (bsq::unused_type,
                   bsq::unused_type,
                   bsq::unused_type) const {
    _flightPeriod._dateRangeEnd = _flightPeriod.getDate();
    //std::cout << "Date Range End: "
    // << _flightPeriod._dateRangeEnd << std::endl;
      
    // Reset the number of seconds
    _flightPeriod._itSeconds = 0;
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the DOW (day of the Week). */
struct store_dow {
  store_dow (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}

  void operator() (std::vector<char> iChar,
                   bsq::unused_type,
                   bsq::unused_type) const {
    std::string lDow (iChar.begin(), iChar.end());
    _flightPeriod._dow = lDow;
    //std::cout << "DOW: " << lDow << std::endl;
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the parsed board point. */
struct store_board_point {
  store_board_point (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
  
  void operator() (std::vector<char> iChar,
                  bsq::unused_type,
                  bsq::unused_type) const { 
    std::string lBoardPoint (iChar.begin(), iChar.end());
    //std::cout << "Board point: " << lBoardPoint << std::endl;

    // If a leg has already been parsed, add it to the FlightPeriod
    if (_flightPeriod._legAlreadyDefined == true) {
      _flightPeriod._legList.push_back (_flightPeriod._itLeg);
    } else {
      _flightPeriod._legAlreadyDefined = true;
    }

    // Set the (new) board point
    _flightPeriod._itLeg._boardPoint = lBoardPoint;
      
    // As that's the beginning of a new leg, the list of cabins must be reset
    _flightPeriod._itLeg._cabinList.clear();

    // Add the airport code if it is not already stored in the airport lists
    _flightPeriod.addAirport (lBoardPoint);
  }
    
  FlightPeriod_T& _flightPeriod;
};
  
/** Store the parsed off point. */
struct store_off_point {
  store_off_point (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}

  void operator() (std::vector<char> iChar,
                   bsq::unused_type,
                   bsq::unused_type) const { 
    std::string lOffPoint (iChar.begin(), iChar.end());
    _flightPeriod._itLeg._offPoint = lOffPoint;
    //std::cout << "Off point: " << lOffPoint << std::endl;

    // Add the airport code if it is not already stored in the airport lists
    _flightPeriod.addAirport (lOffPoint);
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the board time. */
struct store_board_time {
  store_board_time (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (bsq::unused_type,
                   bsq::unused_type,
                   bsq::unused_type) const { 
    _flightPeriod._itLeg._boardTime = _flightPeriod.getTime();

    //std::cout << "Board time: "
    //<< _flightPeriod._itLeg._boardTime << std::endl;
      
    // Reset the number of seconds
    _flightPeriod._itSeconds = 0;

    // Reset the date off-set
    _flightPeriod._dateOffSet = 0;
  }
  
  FlightPeriod_T& _flightPeriod;
};

/** Store the off time. */
struct store_off_time {
  store_off_time (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (bsq::unused_type,
                   bsq::unused_type,
                   bsq::unused_type) const {
    _flightPeriod._itLeg._offTime = _flightPeriod.getTime();

    //std::cout << "Off time: "
    //  << _flightPeriod._itLeg._offTime << std::endl;
      
    // Reset the number of seconds
    _flightPeriod._itSeconds = 0;

    // As the board date off set is optional, it can be set only afterwards,
    // based on the staging date off-set value (_flightPeriod._dateOffSet).
      const boost::gregorian::date_duration lDateOffSet (_flightPeriod._dateOffSet);
      _flightPeriod._itLeg._boardDateOffSet = lDateOffSet;
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the elapsed time. */
struct store_elapsed_time {
  store_elapsed_time (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (bsq::unused_type,
                   bsq::unused_type,
                   bsq::unused_type) const {

    _flightPeriod._itLeg._elapsed = _flightPeriod.getTime();

    //std::cout << "Elapsed time: "
    // << _flightPeriod._itLeg._elapsed << std::endl;
      
    // Reset the number of seconds
    _flightPeriod._itSeconds = 0;

    // As the board date off set is optional, it can be set only afterwards,
    // based on the staging date off-set value (_flightPeriod._dateOffSet).
    const boost::gregorian::date_duration lDateOffSet (_flightPeriod._dateOffSet);
    _flightPeriod._itLeg._offDateOffSet = lDateOffSet;
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the parsed cabin code. */
struct store_cabin_code {
  store_cabin_code (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (char iChar,
                   bsq::unused_type,
                   bsq::unused_type) const { 
    _flightPeriod._itCabin._cabinCode = iChar;
    //std::cout << "Cabin code: " << _flightPeriod._itCabin._cabinCode << std::endl;
  }
    
  FlightPeriod_T& _flightPeriod;
};

/** Store the parsed capacity. */
struct store_capacity {
  store_capacity (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (double iReal,
                   bsq::unused_type,
                   bsq::unused_type) const {  
    _flightPeriod._itCabin._capacity = iReal; 
    //std::cout << "Capacity: " << iReal << std::endl;
    
    // The capacity is the last (according to arrival order) detail
    // of the cabin. Hence, when a capacity is parsed, it means that
    // the full cabin details have already been parsed as well: the
    // cabin can thus be added to the leg.
    _flightPeriod._itLeg._cabinList.push_back (_flightPeriod._itCabin);
  }

  FlightPeriod_T& _flightPeriod;
};

/** Store whether or not all the segments are the same. */
struct store_segment_specificity {
  store_segment_specificity (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {
  }

  void operator() (char iChar,
                   bsq::unused_type,
                   bsq::unused_type) const { 
    if (iChar == '0') {
      _flightPeriod._areSegmentDefinitionsSpecific = false;
    } else {
      _flightPeriod._areSegmentDefinitionsSpecific = true;
    }

    // Do a few sanity checks: the two lists should get exactly the same
    // content (in terms of airport codes). The only difference is that one
    // is a STL set, and the other a STL vector.
    assert (_flightPeriod._airportList.size()
            == _flightPeriod._airportOrderedList.size());
    assert (_flightPeriod._airportList.size() >= 2);
      
    // Since all the legs have now been parsed, we get all the airports
    // and the segments may be built.
    _flightPeriod.buildSegments();
  }

  FlightPeriod_T& _flightPeriod;
};
      
/** Store the board point of the segment. */
struct store_segment_board_point {
  store_segment_board_point (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {
  }

  void operator() (std::vector<char> iChar,
                   bsq::unused_type,
                   bsq::unused_type) const {
    std::string lBoardPoint (iChar.begin(), iChar.end());
    _flightPeriod._itSegment._boardPoint = lBoardPoint;
    //std::cout << "Board point: " << lBoardPoint << std::endl;
  }

  FlightPeriod_T& _flightPeriod;
};

/** Store the off point of the segment. */
struct store_segment_off_point {
  store_segment_off_point (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {
  }

  void operator() (std::vector<char> iChar,
                   bsq::unused_type,
                   bsq::unused_type) const {
    std::string lOffPoint (iChar.begin(), iChar.end());
    _flightPeriod._itSegment._offPoint = lOffPoint;
    //std::cout << "Off point: " << lOffPoint << std::endl;
  }

  FlightPeriod_T& _flightPeriod;
};

/** Store the cabin code of the segment. */
struct store_segment_cabin_code {
  store_segment_cabin_code (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {
  }
    
  void operator() (char iChar,
                   bsq::unused_type,
                   bsq::unused_type) const {
    _flightPeriod._itSegmentCabin._cabinCode = iChar; 
    //std::cout << "Cabin code: " << iChar << std::endl;
  }

  FlightPeriod_T& _flightPeriod;
};

/** Store the classes of the segment-cabin. */
struct store_classes {
  store_classes (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {
  }

  void operator() (std::vector<char> iChar,
                   bsq::unused_type,
                   bsq::unused_type) const {
    std::string lClasses (iChar.begin(), iChar.end());
    _flightPeriod._itSegmentCabin._classes = lClasses;
    //std::cout << "Classes: " << lClasses << std::endl;

    // The list of classes is the last (according to the arrival order
    // within the schedule input file) detail of the segment cabin. Hence,
    // when a list of classes is parsed, it means that the full segment
    // cabin details have already been parsed as well: the segment cabin
    // can thus be added to the segment.
    if (_flightPeriod._areSegmentDefinitionsSpecific == true) {
      _flightPeriod.addSegmentCabin (_flightPeriod._itSegment,
                                     _flightPeriod._itSegmentCabin);
    } else {
      _flightPeriod.addSegmentCabin (_flightPeriod._itSegmentCabin);
    }
  }

  FlightPeriod_T& _flightPeriod;
};
  
/** Mark the end of the flight-period parsing. */
struct do_end_flight {
  do_end_flight (FlightPeriod_T& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (bsq::unused_type,
                   bsq::unused_type,
                   bsq::unused_type) const {
    //std::cout << "End of Flight-Period " << std::endl;

    assert (_flightPeriod._legAlreadyDefined == true);
    _flightPeriod._legList.push_back (_flightPeriod._itLeg);

    // Display the result
    _flightPeriod.display();
      
    // The lists of legs and cabins must be reset
    _flightPeriod._legAlreadyDefined = false;
    _flightPeriod._itLeg._cabinList.clear();

  }

  FlightPeriod_T& _flightPeriod;
};

// /////////// Utilities /////////////

/** 1-digit-integer parser */
bsq::int_parser<unsigned int, 10, 1, 1> int1_p;

/** 2-digit-integer parser */
bsq::uint_parser<int, 10, 2, 2> uint2_p;

/** 4-digit-integer parser */
bsq::uint_parser<int, 10, 4, 4> uint4_p;

/** Up-to-4-digit-integer parser */
bsq::uint_parser<int, 10, 1, 4> uint1_4_p;


///////////////////////////////////////////////////////////////////////////////
//
//  Our flight-period grammar 
//
///////////////////////////////////////////////////////////////////////////////
    /**
       AirlineCode; FlightNumber; DateRangeStart; DateRangeEnd; DOW;
       (list) BoardPoint; OffPoint; BoardTime; DateOffSet; OffTime;
       ElapsedTime;
       (list) CabinCode; Capacity;
       SegmentSpecificty (0 or 1);
       (list) (optional BoardPoint; OffPoint); CabinCode; Classes

       BA; 9; 2007-04-20; 2007-06-30; 0000011;
       LHR; BKK; 22:00; 15:15 / +1; 11:15; F; 5; J; 12; W; 20; Y; 300;
       BKK; SYD; 18:10 / +1; 06:05 / +2; 08:55; F; 5; J; 12; W; 20; Y; 300;
       0; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;
       BA; 9; 2007-04-20; 2007-06-30; 1111100;
       LHR; BKK; 22:00; 15:15 / +1; 11:15; F; 5; J; 12; W; 20; Y; 300;
       BKK; SYD; 18:10 / +1; 06:05 / +2; 08:55; F; 5; J; 12; W; 20; Y; 300;
       1; LHR; BKK; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;
       BKK; SYD; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;
       LHR; SYD; F; FA; J; JCDI; W; WT; Y; YBHKMLSQ;

       Grammar:
       DOW                 ::= int
       FlightKey           ::= AirlineCode ';' FlightNumber
       ';' DateRangeStart ';' DateRangeEnd ';' DOW
       LegKey              ::= BoardPoint ';' OffPoint
       LegDetails          ::= BoardTime
       ';' OffTime 
       ';' Elapsed
       LegCabinDetails     ::= CabinCode ';' Capacity
       Leg                 ::= LegKey ';' LegDetails (';' CabinDetails)+
       SegmentKey          ::= BoardPoint ';' OffPoint
       SegmentCabinDetails ::= CabinCode ';' Classes
       FullSegmentCabinDetails::= (';' SegmentCabinDetails)+
       GeneralSegments     ::= '0' (';' SegmentCabinDetails)+
       SpecificSegments    ::= '1' (';' SegmentKey
       ';' FullSegmentCabinDetails)+
       Segment             ::= GeneralSegment | SpecificSegment
       FlightPeriod        ::= FlightKey (';' Leg)+
       (';' Segment)+ ';' EndOfFlight
       EndOfFlight         ::= ';'
    */

  
////////////////////////////////////////////////////////////////////////////
//  Parser
////////////////////////////////////////////////////////////////////////////
template <typename ITERATOR>
struct FlightPeriodParser : 
  bsq::grammar<ITERATOR, bsa::space_type> {

  FlightPeriod_T _flightPeriod;
    
  FlightPeriodParser () :
    FlightPeriodParser::base_type(start) {

    start = *(comment | flight_period);

    comment = (bsq::lexeme[ (bsq::repeat(2)[bsa::char_("/")]) >> +(bsa::char_ - bsq::eol) >> bsq::eol ]
               | bsq::lexeme[ (bsa::char_("/") >> bsa::char_("*") >> +(bsa::char_ - bsa::char_("*")) >> bsa::char_("*") >> bsa::char_("/")) ])
               ;

    flight_period = flight_key
      >> +( ';' >> leg )
      >> +( ';' >> segment )
      >> flight_period_end[do_end_flight(_flightPeriod)]
      ;

    flight_period_end = ';';

    flight_key = airline_code
      >> ';' >> flight_number
      >> ';' >> date[store_date_range_start(_flightPeriod)] 
      >> ';' >> date[store_date_range_end(_flightPeriod)] 
      >> ';' >> dow
      ;
 
    airline_code =
       bsq::lexeme[ (bsq::repeat(2)[bsa::char_("A-Z")])[store_airline_code(_flightPeriod)] ];
       
    flight_number = uint1_4_p[store_flight_number(_flightPeriod)];
      
    date =  bsq::lexeme[ uint4_p[bp::ref(_flightPeriod._itYear) = bsq::labels::_1]
                         >> '-' >> uint2_p[bp::ref(_flightPeriod._itMonth) =bsq::labels::_1]
                         >> '-' >> uint2_p[bp::ref(_flightPeriod._itDay) = bsq::labels::_1] ]
      ;
    
    dow =
       bsq::lexeme[ (bsq::repeat(7)[bsa::char_("0-1")])[store_dow(_flightPeriod)] ]
        ;

    leg = leg_key >> ';' >> leg_details >> +( ';' >> cabin_details )
        ;
	 
    leg_key =
       bsq::lexeme[ (bsq::repeat(3)[bsa::char_("A-Z")])[store_board_point(_flightPeriod)] ]
      >> ';'
      >>  bsq::lexeme[ (bsq::repeat(3)[bsa::char_("A-Z")])[store_off_point(_flightPeriod)] ]
      ;  
          
    leg_details =
      time[store_board_time(_flightPeriod)]
      >> -(date_offset)
      >> ';' 
      >> time[store_off_time(_flightPeriod)]
      >> -(date_offset)
      >> ';'
      >> time[store_elapsed_time(_flightPeriod)]
      ;

    time =  bsq::lexeme[ uint2_p[bp::ref(_flightPeriod._itHours) = bsq::labels::_1]
                   >> ':' >> uint2_p[bp::ref(_flightPeriod._itMinutes) = bsq::labels::_1]
                   >> !(':' >> (uint2_p)[bp::ref(_flightPeriod._itSeconds) = bsq::labels::_1]) ]
      ;

    date_offset = bsa::char_('/')
      >> (bsa::char_("+") | bsa::char_("-"))
      >> int1_p[bp::ref(_flightPeriod._dateOffSet) = bsq::labels::_1]
      ;    

    cabin_details = bsa::char_("A-Z") [store_cabin_code(_flightPeriod)]
      >> ';' >> bsq::
      double_[store_capacity(_flightPeriod)]
      ;

    segment_key = (bsq::repeat(3)[bsa::char_("A-Z")])[store_segment_board_point(_flightPeriod)]
      >> ';'
      >> (bsq::repeat(3)[bsa::char_("A-Z")])[store_segment_off_point(_flightPeriod)]
      ;

    segment =
      general_segments | specific_segments
      ;
    
    general_segments = 
      bsa::char_('0')[store_segment_specificity(_flightPeriod)]
      >> +(';' >> segment_cabin_details)
      ;

    specific_segments = 
      bsa::char_('1')[store_segment_specificity(_flightPeriod)]
      >> +(';' >> segment_key >> full_segment_cabin_details)
      ;
        
    full_segment_cabin_details =
      +(';' >> segment_cabin_details)
      ;
      
    segment_cabin_details = bsa::char_("A-Z")[store_segment_cabin_code(_flightPeriod)]
      >> ';' >> (bsq::repeat(1,26)[bsa::char_("A-Z")])[store_classes(_flightPeriod)]
      ;
 }
 
  bsq::rule<ITERATOR, bsa::space_type>
  start, comment, flight_period, flight_key, airline_code,
    flight_number, date, dow, leg, leg_key, leg_details,
    time, date_offset, cabin_details, segment, segment_key,
    general_segments, specific_segments, full_segment_cabin_details,
    segment_cabin_details, flight_period_end;
  
};


////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[]) {

  // File to be parsed
  std::ifstream fileToBeParsed("world_schedule.csv", std::ios_base::in);
    
  // Open the file
  if (fileToBeParsed == false) {
    std::cerr << "Error: Could not open input file: world_schedule.csv"
              << std::endl;
    return 1;
  }
    
  // Create an input iterator
  typedef std::istreambuf_iterator<char> base_iterator_type;  
  base_iterator_type inputBegin(fileToBeParsed);  

  // Convert input iterator to forward iterator, usable by spirit parser  
  typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;  
  forward_iterator_type iter =  boost::spirit::make_default_multi_pass(inputBegin);  
  forward_iterator_type end;

  // Define our grammar
  typedef FlightPeriodParser<forward_iterator_type> lFlightPeriodParser;
  lFlightPeriodParser flightParser; 

  // Parse input file
  const bool hasParsingBeenSuccesful =
    bsq::phrase_parse(iter, end, flightParser, bsa::space);

  std::cout << "-------------------------------------------------------------------------------\n";
  if (hasParsingBeenSuccesful == false) {
    std::cout << "Parsing of schedule input failed"
              << std::endl;
  }
  if  (iter != end) {
    std::cout << "Parsing of schedule input failed"
              << std::endl;
  }
  if (hasParsingBeenSuccesful == true && iter == end) {
    std::cout << "Parsing of schedule input succeeded"
              << std::endl;
  }
  std::cout << "-------------------------------------------------------------------------------\n";
    
  return 0;
}
