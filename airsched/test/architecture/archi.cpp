// STL
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ///////////////////////////////////////////////////////////////////////
/** Base class. */
class BaseClass {
public:
  /** Constructor. */
  BaseClass (const std::string& iID) : _id (iID) { }
  /** Display. */
  virtual std::string toString() { return _id; }
  
protected:
  std::string _id;
};

/** Class inheriting from the Base Class. */
template <typename T>
class InheritingClass : public BaseClass {
public:
  /** Type definition. */
  typedef std::vector<T> List_T;
  
  /** Constructor. */
  InheritingClass (const std::string& iID) : BaseClass (iID) { }
  /** Display. */
  std::string toString() {
    std::ostringstream oStr;
    oStr << "ID: " << _id << ", ";
    for (typename List_T::const_iterator it = _list.begin();
         it != _list.end(); ++it) {
      oStr << *it;
    }
    return oStr.str();
  }

  /** Add an item. */
  void addItem (const T& iItem) {
    _list.push_back (iItem);
  }
  
private:
  /** List. */
  List_T _list;
};


// ///////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[]) {

  BaseClass lBaseClass ("BaseClass");
  InheritingClass<std::string> lInheritingClass ("InheritingClass");
  lInheritingClass.addItem ("Hello ");
  lInheritingClass.addItem ("Anh Quan");
  lInheritingClass.addItem ("!");
  
  std::cout << "Base class: " << lBaseClass.toString() << std::endl;
  std::cout << "Inheriting class: " << lInheritingClass.toString() << std::endl;
  
  return 0;
}
