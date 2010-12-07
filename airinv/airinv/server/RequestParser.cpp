// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// AirInv
#include <airinv/server/RequestParser.hpp>
#include <airinv/server/Request.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  RequestParser::RequestParser()
    : state_(method_start) {
  }

  // //////////////////////////////////////////////////////////////////////
  void RequestParser::reset() {
    state_ = method_start;
  }

  // //////////////////////////////////////////////////////////////////////
  boost::tribool RequestParser::consume (Request& req, char input) {

    switch (state_) {

    case method_start:
      if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return false;
      
      } else {
        state_ = method;
        req.method.push_back(input);
        return boost::indeterminate;
      }
    
    case method:
      if (input == ' ') {
        state_ = uri;
        return boost::indeterminate;
      
      } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return false;
      
      } else {
        req.method.push_back(input);
        return boost::indeterminate;
      }
    
    case uri_start:
      if (is_ctl(input)) {
        return false;
      
      } else {
        state_ = uri;
        req.uri.push_back(input);
        return boost::indeterminate;
      }
    
    case uri:
      if (input == ' ') {
        state_ = http_version_h;
        return boost::indeterminate;
      
      } else if (is_ctl(input)) {
        return false;
      
      } else {
        req.uri.push_back(input);
        return boost::indeterminate;
      }
    
    case http_version_h:
      if (input == 'H') {
        state_ = http_version_t_1;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_t_1:
      if (input == 'T') {
        state_ = http_version_t_2;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_t_2:
      if (input == 'T') {
        state_ = http_version_p;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_p:
      if (input == 'P') {
        state_ = http_version_slash;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_slash:
      if (input == '/') {
        req.http_version_major = 0;
        req.http_version_minor = 0;
        state_ = http_version_major_start;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_major_start:
      if (is_digit(input)) {
        req.http_version_major = req.http_version_major * 10 + input - '0';
        state_ = http_version_major;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_major:
      if (input == '.') {
        state_ = http_version_minor_start;
        return boost::indeterminate;
      
      } else if (is_digit(input)) {
        req.http_version_major = req.http_version_major * 10 + input - '0';
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_minor_start:
      if (is_digit(input)) {
        req.http_version_minor = req.http_version_minor * 10 + input - '0';
        state_ = http_version_minor;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case http_version_minor:
      if (input == '\r') {
        state_ = expecting_newline_1;
        return boost::indeterminate;
      
      } else if (is_digit(input)) {
        req.http_version_minor = req.http_version_minor * 10 + input - '0';
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case expecting_newline_1:
      if (input == '\n') {
        state_ = header_line_start;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case header_line_start:
      if (input == '\r') {
        state_ = expecting_newline_3;
        return boost::indeterminate;
      
      } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return false;
      
      } else {
        state_ = header_name;
        return boost::indeterminate;
      }
    
    case header_lws:
      if (input == '\r') {
        state_ = expecting_newline_2;
        return boost::indeterminate;
      
      } else if (input == ' ' || input == '\t') {
        return boost::indeterminate;
      
      } else if (is_ctl(input)) {
        return false;
      
      } else {
        state_ = header_value;
        return boost::indeterminate;
      }
    
    case header_name:
      if (input == ':') {
        state_ = space_before_header_value;
        return boost::indeterminate;
      
      } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return false;
      
      } else {
        return boost::indeterminate;
      }
    
    case space_before_header_value:
      if (input == ' ') {
        state_ = header_value;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case header_value:
      if (input == '\r') {
        state_ = expecting_newline_2;
        return boost::indeterminate;
      
      } else if (is_ctl(input)) {
        return false;
      
      } else {
        return boost::indeterminate;
      }
    
    case expecting_newline_2:
      if (input == '\n') {
        state_ = header_line_start;
        return boost::indeterminate;
      
      } else {
        return false;
      }
    
    case expecting_newline_3:
      return (input == '\n');
    
    default:
      return false;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool RequestParser::is_char(int c) {
    return c >= 0 && c <= 127;
  }

  // //////////////////////////////////////////////////////////////////////
  bool RequestParser::is_ctl(int c) {
    return (c >= 0 && c <= 31) || (c == 127);
  }

  // //////////////////////////////////////////////////////////////////////
  bool RequestParser::is_tspecial(int c) {
    switch (c) {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
      return true;
    default:
      return false;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool RequestParser::is_digit(int c) {
    return c >= '0' && c <= '9';
  }

}
