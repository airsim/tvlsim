// STL
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// Boost Spirit (Parsing)
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/lambda/lambda.hpp>


typedef std::string::const_iterator iterator_t;
/** Flight-Period. */
struct FlightPeriod_T {
  // Attributes
  std::string _airlineCode;
  unsigned int _flightNumber;
  boost::gregorian::date _dateRangeStart;
  boost::gregorian::date _dateRangeEnd;
  std::string _dow;
  std::string _cabinCode;
};

/** Store the parsed airline code. */
template <typename FLIGHT_PERIOD>
struct store_airline_code {
  store_airline_code (FLIGHT_PERIOD& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (iterator_t iStr, iterator_t iStrEnd) const { 
    std::string lAirlineCode (iStr, iStrEnd);
    _flightPeriod._airlineCode = lAirlineCode;
    // std::cout << "Airline code: " << lAirlineCode << std::endl;
  }
    
  FLIGHT_PERIOD& _flightPeriod;
};

/** Store the parsed cabin code. */
template <typename FLIGHT_PERIOD>
struct store_cabin_code {
  store_cabin_code (FLIGHT_PERIOD& ioFlightPeriod)
    : _flightPeriod (ioFlightPeriod) {}
    
  void operator() (char iChar) const { 
    _flightPeriod._cabinCode = iChar;
    // std::cout << "Cabin code: " << lCabinCode << std::endl;
  }
    
  FLIGHT_PERIOD& _flightPeriod;
};
  

// /////////// Utilities /////////////

/** 1-digit-integer parser */
boost::spirit::qi::uint_parser<int, 10, 1, 1> int1_p;

/** 2-digit-integer parser */
boost::spirit::qi::uint_parser<int, 10, 2, 2> uint2_p;

/** 4-digit-integer parser */
boost::spirit::qi::uint_parser<int, 10, 4, 4> uint4_p;

/** Up-to-4-digit-integer parser */
boost::spirit::qi::uint_parser<int, 10, 1, 4> uint1_4_p;

  
////////////////////////////////////////////////////////////////////////////
//  Parser
////////////////////////////////////////////////////////////////////////////
template <typename ITERATOR, typename FLIGHT_PERIOD>
struct FlightPeriodParser : 
  boost::spirit::qi::grammar<ITERATOR, boost::spirit::ascii::space_type > {

  typedef store_airline_code<FLIGHT_PERIOD> store_airline_code_t;
  typedef store_cabin_code<FLIGHT_PERIOD> store_cabin_code_t;
  
  FLIGHT_PERIOD _t;
    
  FlightPeriodParser () :
    FlightPeriodParser::base_type(start) {

    using boost::spirit::ascii::string;
    using boost::spirit::ascii::char_;
    using boost::spirit::qi::lexeme;
    using boost::spirit::qi::repeat;
    using boost::spirit::qi::double_;
    using boost::lambda::_1;


    start = (comment | flight_period) ;

    comment = lexeme[ (repeat(2)[char_("/")]) ] >> *(char_);

    flight_period = airline_code
      >> ';' >> flight_number
      >> ';' >> date
      >> ';' >> date
      >> ';' >> dow
      >> ';'
      >> leg_list
      >> segment_list
      ;
      
    airline_code = lexeme[ (repeat(2)[char_("A-Z")]) ];
    
      
    flight_number = uint1_4_p;
      
    date = lexeme[ uint4_p
        >> '-' >> uint2_p
        >> '-' >> uint2_p ]
        ;

    dow =
        lexeme[ repeat(7)[char_("0-1")] ]
        ;

    leg_list = +(leg);
    
    leg = leg_key 
        >> leg_details 
        >> +(cabin_details)
        ;
	 
    leg_key =
      lexeme[ repeat(3)[char_("A-Z")] ]
      >> ';'
      >> lexeme[ repeat(3)[char_("A-Z")] ]
      >> ';'
      ;
	 
    leg_details =
      time
      >> ';' >> time
      >> ';' >> time
      >> ';'
      ;

    time = lexeme[ uint2_p
      >> ':' >> uint2_p
       >> !(':' >> uint2_p) ]
      ;        
             
    cabin_details = char_("A-Z") [store_cabin_code_t(_t)]
      >> ';' >> double_
      >> ';'
      ;

    segment_key =
      repeat(3)[char_("0-9A-Z")]
      >> ';'
      >>repeat(3)[char_("0-9A-Z")]
      >> ';'
      ;
    
    segment_list = +(segment);
	 
    segment =
      general_segments | specific_segments
      ;

    general_segments =
      char_('0')
      >> ';'
      >> +(segment_cabin_details)
      ;

    specific_segments =
      char_('1')
      >> ';'
      >> +(segment_key >> full_segment_cabin_details)
      ;
      
    full_segment_cabin_details =
      +(segment_cabin_details)
      ;
      
    segment_cabin_details =
      char_("A-Z")
      >> ';' >> (repeat(1,26)[char_("A-Z")])
      >> ';'
      ;
 
  }
 
  boost::spirit::qi::rule<ITERATOR, boost::spirit::ascii::space_type> start,
    comment, flight_period, airline_code, flight_number, date, dow, leg, leg_list,
    leg_key, leg_details, time, cabin_details, segment, segment_list, segment_key,
    general_segments, specific_segments, full_segment_cabin_details, segment_cabin_details;
  
};


////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[]) {

  char const* filename = "world_schedule.csv";

  std::ifstream in(filename, std::ios_base::in);

 if (!in) {
   std::cerr << "Error: Could not open input file: "
             << filename << std::endl;
   return 1;
 }

 typedef FlightPeriodParser<std::string::const_iterator, FlightPeriod_T > lFlightPeriodParser;
 lFlightPeriodParser flightParser; // Our grammar
 
 std::string::const_iterator iter;
 std::string::const_iterator end;
 
 std::string line;
 bool r = true;
 using boost::spirit::ascii::space;

 
 while (r && std::getline(in, line)) {
   iter = line.begin();
   end = line.end();
   r = phrase_parse(iter, end, flightParser, space);
   if (iter != end) {
     r = false;
   } 
 }

 if (r && iter == end) {
   std::cout << "-------------------------\n";
   std::cout << "Parsing succeeded\n";
   std::cout << "-------------------------\n";
   return 0;
 } else {
   std::cout << "-------------------------\n";
   std::cout << "Parsing failed\n";
   std::cout << "-------------------------\n";
 }
 return 0;
}
