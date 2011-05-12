/*=============================================================================
    Copyright (c) 2010 Roland Bock

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  A runtime-configurable date parser
//
//  [ RB July 2010 ]     spirit2
//  See also: http://lists.boost.org/Archives/boost/2011/05/181622.php
///////////////////////////////////////////////////////////////////////////////
#include <boost/shared_ptr.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
namespace encoding = boost::spirit::iso8859_1;

namespace date_parser
{
   ///////////////////////////////////////////////////////////////////////////////
   //  Very simple struct for the date information we want to parse
   ///////////////////////////////////////////////////////////////////////////////
   //[tutorial_date_struct
   struct date_struct
   {
      date_struct(): 
         year(0), 
         month(0), 
         day(0)
      {}

      unsigned year;
      unsigned month;
      unsigned day;
   };
   //]

   ///////////////////////////////////////////////////////////////////////////////
   //  Interface for elements of the configurable date parser
   ///////////////////////////////////////////////////////////////////////////////
   //[tutorial_element
   template <typename Iterator>
   class element
   {
   public:
      element() {}

      virtual ~element()
      {}

      virtual bool parse(date_struct& date, Iterator& begin, const Iterator& end) const = 0;
   };
   //]

   static BOOST_AUTO(skipper, encoding::space | encoding::punct);

   ///////////////////////////////////////////////////////////////////////////////
   //  Year element
   //  Parses a year (4 digit unsigned int), checking the configured limits
   ///////////////////////////////////////////////////////////////////////////////
   template <typename Iterator>
   class element_year: public element<Iterator>
   {
   public:
      element_year(const unsigned _min, const unsigned _max): 
         min(_min), 
         max(_max) 
      {}

      virtual bool parse(date_struct& date, Iterator& begin, const Iterator& end) const
      {
         using qi::rule;
         using qi::uint_parser;
         using qi::_pass;
         using qi::_r1;
         using qi::_r2;
         using qi::_1;
         using qi::_val;
         using qi::phrase_parse;

         ///////////////////////////////////////////////////////////////////////////////
         //  Rule to parse a year:
         //    * 4 digit unsigned
         //    * min <= value <= max
         //  Note that we have to explicitly make the assignment in the second semantic action,
         //  because we overruled default semantic action with the check for limits.
         ///////////////////////////////////////////////////////////////////////////////
         //[tutorial_rule_year
         static rule<Iterator, unsigned(unsigned min, unsigned max)> rule_year = uint_parser<unsigned, 10, 4, 4>()
                                                                                 [_pass = (_r1 <= _1 && _1 <= _r2)]
                                                                                 [_val = _1]; 
         //]

         ///////////////////////////////////////////////////////////////////////////////
         //  Using the year rule, to parse into the year member of the date struct
         ///////////////////////////////////////////////////////////////////////////////
         //[tutorial_parse_year
         const bool result = 
            phrase_parse(begin, end,
                  rule_year(min, max), // here we supply the min/max attribute values
                  skipper,
                  date.year // reference to the year member of a date_struct
                  );
         //]

         if (!result)
            std::cout << "Failed to parse year\n";

         return result;
      }

   private:
      unsigned min;
      unsigned max;
   };

   ///////////////////////////////////////////////////////////////////////////////
   //  Month element
   //  Parses a month (1 or 2 digit unsigned int), checking the fixed limits
   ///////////////////////////////////////////////////////////////////////////////
   template <typename Iterator>
   class element_month: public element<Iterator>
   {
   public:
      virtual bool parse(date_struct& date, Iterator& begin, const Iterator& end) const
      {
         using qi::rule;
         using qi::uint_parser;
         using qi::_pass;
         using qi::_1;
         using qi::_val;
         using qi::phrase_parse;

         ///////////////////////////////////////////////////////////////////////////////
         //  Rule to parse a month:
         //    * 1 - 2 digit unsigned
         //    * 1 <= value <= 12
         //  Note that we have to explicitly make the assignment in the second semantic action,
         //  because we overruled default semantic action with the check for limits.
         ///////////////////////////////////////////////////////////////////////////////
         //[tutorial_rule_month
         static rule<Iterator, unsigned()> rule_month = uint_parser<unsigned, 10, 1, 2>()
                                                        [_pass = (1u <= _1 && _1 <= 12u)]
                                                        [_val = _1]; 
         //]

         const bool result = 
            phrase_parse(begin, end,
                  rule_month,
                  skipper,
                  date.month
                  );

         if (!result)
            std::cout << "Failed to parse month\n";

         return result;
      }
   };

   ///////////////////////////////////////////////////////////////////////////////
   //  Day element
   //  Parses a day (1 or 2 digit unsigned int), checking the fixed limits
   ///////////////////////////////////////////////////////////////////////////////
   template <typename Iterator>
   class element_day: public element<Iterator>
   {
   public:
      virtual bool parse(date_struct& date, Iterator& begin, const Iterator& end) const
      {
         using qi::rule;
         using qi::uint_parser;
         using qi::_pass;
         using qi::_1;
         using qi::_val;
         using qi::phrase_parse;

         ///////////////////////////////////////////////////////////////////////////////
         //  Rule to parse a day:
         //    * 1 - 2 digit unsigned
         //    * 1 <= value <= 31
         //  Note that we have to explicitly make the assignment in the second semantic action,
         //  because we overruled default semantic action with the check for limits.
         ///////////////////////////////////////////////////////////////////////////////
         //[tutorial_rule_day
         static rule<Iterator, unsigned()> rule_day = uint_parser<unsigned, 10, 1, 2>()
                                                      [_pass = (1u <= _1 && _1 <= 31u)]
                                                      [_val = _1]; 
         //]

         const bool result = phrase_parse(begin, end,
               rule_day,
               skipper,
               date.day
               );

         if (!result)
            std::cout << "Failed to parse day\n";

         return result;
      }
   };

   ///////////////////////////////////////////////////////////////////////////////
   //  Sequence operator
   //  Sequentially parses two elements (which could in turn be sequences, of course)
   ///////////////////////////////////////////////////////////////////////////////
   //[tutorial_operator_sequence
   template <typename Iterator>
   class operator_sequence: public element<Iterator>
   {
   public:
      operator_sequence(boost::shared_ptr<element<Iterator> > _first, boost::shared_ptr<element<Iterator> > _second):
         first(_first),
         second(_second)
      {}

      virtual bool parse(date_struct& date, Iterator& begin, const Iterator& end) const
      {
         return first->parse(date, begin, end) && second->parse(date, begin, end);
      }

   private:
      boost::shared_ptr<element<Iterator> > first;
      boost::shared_ptr<element<Iterator> > second;
   };
   //]

   ///////////////////////////////////////////////////////////////////////////////
   //  Grammar for parsing the date format
   //  Constructs a parser made of sequences and data elements
   ///////////////////////////////////////////////////////////////////////////////
   //[tutorial_grammar_format
   template <typename Iterator>
   struct grammar_format : qi::grammar<Iterator, boost::shared_ptr<element<Iterator> >(), encoding::space_type>
   {
      grammar_format():
         grammar_format::base_type(sequence)
      {
         using qi::eps;
         using qi::lit;
         using qi::uint_;
         using qi::_a;
         using qi::_b;
         using qi::_1;
         using qi::_2;
         using qi::_pass;
         using qi::_val;

         using phoenix::construct;
         using phoenix::new_;

         year = 
            (
               eps[_a = 1000u] >> eps[_b = 9999u]
               >> lit('Y')
               >> - 
               ( 
                     lit('(') 
                  >> uint_[_pass = (_a <= _1 && _1 <= _b)][_a = _1]
                  >> lit(',') 
                  >> uint_[_pass = (_a <= _1 && _1 <= _b)][_b = _1]
                  >> lit(')') 
               )
            )
            [_val = construct<boost::shared_ptr<element<Iterator> > >(new_<element_year<Iterator> >(_a, _b))];

         month = lit('M')[_val = construct<boost::shared_ptr<element<Iterator> > >(new_<element_month<Iterator> >())];

         day   = lit('D')[_val = construct<boost::shared_ptr<element<Iterator> > >(new_<element_day<Iterator> >())];

         term = year | month | day;

         sequence = (term >> sequence)[_val = construct<boost::shared_ptr<element<Iterator> > >(new_<operator_sequence<Iterator> >(_1, _2))]
            | term[_val = _1];

      }

      qi::rule<Iterator, boost::shared_ptr<element<Iterator> >(), encoding::space_type, qi::locals<unsigned, unsigned> > year;
      qi::rule<Iterator, boost::shared_ptr<element<Iterator> >(), encoding::space_type> month;
      qi::rule<Iterator, boost::shared_ptr<element<Iterator> >(), encoding::space_type> day;
      qi::rule<Iterator, boost::shared_ptr<element<Iterator> >(), encoding::space_type> term;
      qi::rule<Iterator, boost::shared_ptr<element<Iterator> >(), encoding::space_type> sequence;
   };
   //]


   ///////////////////////////////////////////////////////////////////////////////
   //  Method for reading the date format and constructing the parser
   ///////////////////////////////////////////////////////////////////////////////
   template <typename Iterator>
   bool read_format(Iterator iter, Iterator end, boost::shared_ptr<element<Iterator> >& root)
   {
      grammar_format<Iterator> grammar;

      bool success = phrase_parse(iter, end, grammar, encoding::space, root);

      return success && iter == end;
   }

}

int main()
{
   std::cout << "////////////////////////////////////////////////////////////////\n";
   std::cout << " A runtime-configurable date parser built with Boost::Spirit...\n";
   std::cout << "////////////////////////////////////////////////////////////////\n";
   std::cout << "\n";
   std::cout << "First, define a date format of 'Y', 'Y(min, max)', 'M' and 'D' \n";
   std::cout << "with \n";
   std::cout << "  - min: unsigned int from the interval [1000,9999] \n";
   std::cout << "  - max: unsigned int from the interval [1000,9999] \n";
   std::cout << "  - min <= max \n";
   std::cout << "\n";
   std::cout << "Examples:\n";
   std::cout << "   MDY            would allow to parse dates like 03/17/1203\n";
   std::cout << "   MDY(1970,2020) would allow to parse dates like 03/17/2010\n";
   std::cout << "                  but not                         03/17/1969\n";
   std::cout << "   DMY            would allow to parse dates like 17.03.2010\n";
   std::cout << "   YMD            would allow to parse dates like 2010-03-17\n";
   std::cout << "   YYMD           would allow to parse dates like 2009-2010 03/17\n";
   std::cout << "Type [q] or [Q] and [Enter] to quit\n\n";

   std::string str;
   std::string::const_iterator begin;
   std::string::const_iterator end;
   while (getline(std::cin, str))
   {
      if (str.empty() || str[0] == 'q' || str[0] == 'Q')
         break;

      boost::shared_ptr<date_parser::element<std::string::const_iterator> > root;
      begin = str.begin();
      end = str.end();
      if (date_parser::read_format(begin, end, root))
      {
         std::cout << "-------------------------\n";
         std::cout << "Format parsing succeeded\n";
         std::cout << "-------------------------\n";

         std::cout << "Now, enter date strings to see how your format is working\n";
         std::cout << "  - year:  4 digits unsigned int within the configured limits\n";
         std::cout << "  - month: 1 - 2 digits unsigned int within the interval [1..12]\n";
         std::cout << "  - day:   1 - 2 digits unsigned int within the interval [1..31]\n";
         std::cout << "An empty line will allow you to reconfigure the parser.\n\n";
         std::cout << "Type [q] or [Q] and [Enter] to quit\n\n";

         while (getline(std::cin, str))
         {
            if (str.empty())
            {
               std::cout << "-------------------------\n";
               std::cout << "Please enter a new format\n";
               std::cout << "-------------------------\n";

               break;
            }

            if (str[0] == 'q' || str[0] == 'Q')
            {
               std::cout << "Bye... :-) \n\n";
               return 0;
            }

            date_parser::date_struct date;
            begin = str.begin();
            end = str.end();
            std::complex<double> c;
            if (root->parse(date, begin, end))
            {
               std::cout << "-------------------------\n";
               std::cout << "Date parsing succeeded\n";
               std::cout << "Year: " << date.year << ", Month: " << date.month << ", Day: " << date.day << std::endl;
               std::cout << "-------------------------\n";
            }
            else
            {
               std::cout << "-------------------------\n";
               std::cout << "Date parsing failed\n";
               std::cout << "-------------------------\n";
            }
         }
      }
      else
      {
         std::cout << "-------------------------\n";
         std::cout << "Format parsing failed\n";
         std::cout << "Please try again!\n";
         std::cout << "-------------------------\n";
      }
   }

   std::cout << "Bye... :-) \n\n";
   return 0;
}

