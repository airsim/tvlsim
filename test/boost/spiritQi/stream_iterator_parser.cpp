//  Copyright (c) 2010 Peter Schueller
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;


////////////////////////////////////////////////////////////////////////////
//  Parser
////////////////////////////////////////////////////////////////////////////
template <typename iterator_t> 
bool parse(iterator_t& iStr, iterator_t& iStrEnd,
           std::vector<double>& output) {

  // Parse list of doubles from input stream
  bool r = qi::phrase_parse(
    iStr, iStrEnd,                                                // Iterators
    *(qi::double_ >> ';'),                                        // Recognize list of doubles
    ascii::space | '#' >> *(ascii::char_ - qi::eol) >> qi::eol,   // Skip comment
    output);                                                      // Store double into this object

  // Return result of parsing
  return r;

}

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[]) {
  
  std::string lFilename("good_input.csv");
  std::ifstream File;

  // Read the command-line parameters
  if (argc >= 1 && argv[1] != NULL) {
    lFilename = argv[1];
    File.open (argv[1],std::ios_base::in);
  } else {
    // Default File to be parsed
    File.open ("good_input.csv",std::ios_base::in);
  }
  
  // Check the File existence
  if (!File) {
     std::cerr << "Unknown file named" << lFilename << std::endl;
     return 1;
  }

  // Iterate over stream input  
  typedef std::istreambuf_iterator<char> base_iterator_type;  
  base_iterator_type in_begin(File.rdbuf());
  
  // Convert input iterator to forward iterator, usable by spirit parser  
  typedef boost::spirit::multi_pass<base_iterator_type>forward_iterator_type;  
  forward_iterator_type fwd_begin = 
    boost::spirit::make_default_multi_pass(in_begin);  
  forward_iterator_type fwd_end;
  
  // Prepare output
  std::vector<double> output;
  
  try {
    bool r = parse(fwd_begin, fwd_end, output);
    // Error detection
    if( !r || fwd_begin != fwd_end ) {
      throw std::runtime_error("parse error in "+lFilename);
    } else {
      std::cout << "--------------------------------------------\n";
      std::cout << "Parsing of the file " << lFilename <<" succeeded\n";
      std::cout << "Got: ";
      for (unsigned int i = 0; i < output.size(); i++) {
        std::cout << output[i] << " ";
      }
      std::cout << "\n--------------------------------------------\n";
    }
  } catch(const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  return 0;
}

