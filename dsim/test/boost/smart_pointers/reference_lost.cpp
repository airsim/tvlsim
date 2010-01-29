// STL
#include <cassert>
#include <iostream>
// Boost
#include <boost/shared_ptr.hpp>

/** Simple object. */
class Object {
public:
  Object() {
    std::cout << "Object constructor" << std::endl;
  }
  Object(const Object&) {
    std::cout << "Object copy constructor" << std::endl;
  }
  ~Object() {
    std::cout << "Object destructor" << std::endl;
  }
  void display() const {
    std::cout << "Description: " << _description << std::endl;
  }

private:
  /** Description. */
  std::string _description;
};


/** Pointer on the STDAIR Service handler. */
typedef boost::shared_ptr<Object> ObjectPtr_T;


// ////////////////////// M A I N ///////////////////////
int main (int argc, char* argv[]) {

  Object* lObject_ptr = new Object;
  assert (lObject_ptr != NULL);
  Object& lObject = *lObject_ptr;

  std::cout << "The reference is good?" << std::endl;
  lObject.display();
  std::cout << "Yes, the reference is good!" << std::endl;

  {
    ObjectPtr_T lObjectSmartPtr = ObjectPtr_T (lObject_ptr);
  }

  std::cout << "The reference is still good?" << std::endl;
  lObject.display();
  std::cout << "Yes, the reference is still good!" << std::endl;
  
  return 0;
}
