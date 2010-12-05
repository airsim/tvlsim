// STL
#include <iostream>

int main (int argc, char* argv[]) {

  // Const pointer on non const element
  int i1 = 0;
  const int* myIntPtr1 = NULL;
  myIntPtr1 = &i1;
  // *myIntPtr1 = 3; => compilation error, as the pointer is const
  std::cout << "Integer value: " << *myIntPtr1 << std::endl;

  // Const pointer on const element
  const int i2 = 0;
  const int* myIntPtr2 = NULL;
  myIntPtr2 = &i2;
  // *myIntPtr2 = 3; => compilation error, as the pointer is const
  std::cout << "Integer value: " << *myIntPtr2 << std::endl;

  // Non const pointer on non const element
  int i3 = 0;
  int* myIntPtr3 = NULL;
  myIntPtr3 = &i3;
  *myIntPtr3 = 3;
  std::cout << "Integer value: " << *myIntPtr3 << std::endl;

  // Non const pointer on non const element
  const int i4 = 0;
  int* myIntPtr4 = NULL;
  // myIntPtr4 = &i4; => compilation error because we cannot take a
  // non-const pointer on a const element
  // *myIntPtr4 = 3;
  // std::cout << "Integer value: " << *myIntPtr4 << std::endl;

  return 0;
}

