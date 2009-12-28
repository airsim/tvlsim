/*=============================================================================
  Copyright (c) 2002-2003 Joel de Guzman
  http://spirit.sourceforge.net/

  Use, modification and distribution is subject to the Boost Software
  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  =============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  This calculator example demontrates the use of subrules.
//  This is discussed in the "Subrule" chapter in the Spirit User's Guide.
//
//  [ JDG 4/11/2002 ]
//
///////////////////////////////////////////////////////////////////////////////

//#define BOOST_SPIRIT_DEBUG        // define this for debug output
#include <boost/spirit/home/classic/core.hpp>
#include <iostream>
#include <string>

using namespace boost::spirit::classic;

///////////////////////////////////////////////////////////////////////////////
//
//  Semantic actions
//
///////////////////////////////////////////////////////////////////////////////
namespace {
  
  void do_int (char const* str, char const* end) {
    std::string  s(str, end);
    std::cout << "PUSH(" << s << ')' << std::endl;
  }

  void    do_add(char const*, char const*)    { std::cout << "ADD\n"; }
  void    do_subt(char const*, char const*)   { std::cout << "SUBTRACT\n"; }
  void    do_mult(char const*, char const*)   { std::cout << "MULTIPLY\n"; }
  void    do_div(char const*, char const*)    { std::cout << "DIVIDE\n"; }
  void    do_neg(char const*, char const*)    { std::cout << "NEGATE\n"; }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Our calculator grammar (using subrules)
//
///////////////////////////////////////////////////////////////////////////////
struct calculator : public grammar<calculator> {
  template <typename ScannerT>
  struct definition {

    definition(calculator const& /*self*/) {
      first = (

               expression =
               term
               >> *(   ('+' >> term)[&do_add]
                       |   ('-' >> term)[&do_subt]
                       )
               ,

               term =
               factor
               >> *(   ('*' >> factor)[&do_mult]
                       |   ('/' >> factor)[&do_div]
                       )
               ,

               factor
               =   lexeme_d[(+digit_p)[&do_int]]
               |   '(' >> expression >> ')'
               |   ('-' >> factor)[&do_neg]
               |   ('+' >> factor)
               );

      BOOST_SPIRIT_DEBUG_NODE(first);
      BOOST_SPIRIT_DEBUG_NODE(expression);
      BOOST_SPIRIT_DEBUG_NODE(term);
      BOOST_SPIRIT_DEBUG_NODE(factor);
    }

    subrule<0>  expression;
    subrule<1>  term;
    subrule<2>  factor;

    rule<ScannerT> first;
    rule<ScannerT> const&
    start() const { return first; }
  };
};

///////////////////////////////////////////////////////////////////////////////
//
//  Main program
//
///////////////////////////////////////////////////////////////////////////////
int main() {
  std::cout << "/////////////////////////////////////////////////////////\n\n";
  std::cout << "\t\tA calculator using subrules...\n\n";
  std::cout << "/////////////////////////////////////////////////////////\n\n";
  std::cout << "Type an expression...or [q or Q] to quit\n\n";

  calculator calc;    //  Our parser

  std::string str;
  while (getline (std::cin, str)) {
    if (str.empty() || str[0] == 'q' || str[0] == 'Q') {
      break;
    }

    parse_info<> info = parse(str.c_str(), calc, space_p);

    if (info.full) {
      std::cout << "-------------------------" << std::endl;
      std::cout << "Parsing succeeded" << std::endl;
      std::cout << "-------------------------" << std::endl;
            
    } else {
      std::cout << "-------------------------" << std::endl;
      std::cout << "Parsing failed" << std::endl;
      std::cout << "stopped at: \": " << info.stop << "\"" << std::endl;
      std::cout << "-------------------------" << std::endl;
    }
  }

  std::cout << "Bye... :-)" << std::endl << std::endl;
  return 0;
}
