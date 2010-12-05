#ifndef __B_HPP_
#define __B_HPP_
//
// That program should not compile, as there is a loop-dependency:
// A depends on B, which in turn depends on A. If you want a clear
// picture of what the compilation error is "unknow type" (or something
// similar), you can just use the pre-processor (g++ -E), and look at the
// corresponding source file.
//
// Note that the #ifdef pre-processor macros hinder to expand the header
// file (and hence the full definition of the A class), the second time it
// is included. However, it would be a very bad idea to remove those #ifdef
// macros. Instead, just forward declare A, so that the loop-dependency is
// cleared/removed (uncomment the following forward declaration of A).
//

// B.hpp
#include "A.hpp"

// class A;

class B {

private:
  A* _a;
};
#endif // __B_HPP_
