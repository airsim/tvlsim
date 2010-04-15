// demo.cpp
//
// (C) Copyright 2002-4 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// STL
#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
// Boost.Serialization
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
// Boost.Smart_Pointers
#include <boost/scoped_ptr.hpp>

/////////////////////////////////////////////////////////////
// The intent of this program is to serve as a tutorial for
// users of the serialization package.  An attempt has been made
// to illustrate most of the facilities of the package.  
//
// The intent is to create an example suffciently complete to
// illustrate the usage and utility of the package while
// including a minimum of other code. 
//
// This illustration models the bus system of a small city.
// This includes, multiple bus stops,  bus routes and schedules.
// There are different kinds of stops.  Bus stops in general will
// will appear on multiple routes.  A schedule will include
// muliple trips on the same route.

/////////////////////////////////////////////////////////////
// gps coordinate
//
// Illustrates serialization for a simple type
//
class gps_position {
  friend std::ostream& operator<< (std::ostream& os, const gps_position& gp);
  friend class boost::serialization::access;

  int degrees;
  int minutes;
  float seconds;

  /** Serialize */
  template<class Archive>
  void serialize (Archive& ar, const unsigned int iFileVersion) {
    ar & degrees & minutes & seconds;
  }
  
public:
  // every serializable class needs a constructor
  gps_position() {}
  gps_position(int _d, int _m, float _s) : 
    degrees(_d), minutes(_m), seconds(_s) {
  }
};

/** Dump into an output stream */
std::ostream& operator<< (std::ostream& os, const gps_position& gp) {
  return os << ' ' << gp.degrees << (unsigned char)186 << gp.minutes
            << '\'' << gp.seconds << '"';
}

/////////////////////////////////////////////////////////////
// One bus stop
//
// illustrates serialization of serializable members
//
class bus_stop;

/** Shared pointer on the bus_stop class. */
typedef boost::shared_ptr<bus_stop> BusStopPtr_T;

/** Bus stop. */
class bus_stop {
  friend class boost::serialization::access;
  friend std::ostream& operator<< (std::ostream &os, const bus_stop& gp);
  virtual std::string description() const = 0;
  
  gps_position latitude;
  gps_position longitude;

  /** Serialize. */
  template<class Archive>
  void serialize (Archive& ar, const unsigned int version) {
    ar & latitude;
    ar & longitude;
  }
  
protected:
  /** Constructor. */
  bus_stop (const gps_position& _lat, const gps_position& _long)
    : latitude(_lat), longitude(_long) {
  }
  
public:
  bus_stop() {}
  virtual ~bus_stop() {}
};

// //////////////////////////////////////////////////////////
BOOST_SERIALIZATION_ASSUME_ABSTRACT (bus_stop)

/** Dump into an output stream */
std::ostream& operator<< (std::ostream& os, const bus_stop& bs) {
  return os << bs.latitude << bs.longitude << ' ' << bs.description();
}


/////////////////////////////////////////////////////////////
// Several kinds of bus stops
//
// illustrates serialization of derived types
//

/** Bus stop corner. */
class bus_stop_corner : public bus_stop {
  friend class boost::serialization::access;

  std::string street1;
  std::string street2;

  /** Describe. */
  virtual std::string description() const {
    return street1 + " and " + street2;
  }

  /** Serialize. */
  template<class Archive>
  void serialize (Archive& ar, const unsigned int version) {
    // save/load base class information
    ar & boost::serialization::base_object<bus_stop> (*this);
    ar & street1 & street2;
  }

public:
  /** Constructor. */
  bus_stop_corner() {}
  bus_stop_corner (const gps_position& _lat, const gps_position& _long,
                   const std::string& _s1, const std::string& _s2)
    : bus_stop (_lat, _long), street1 (_s1), street2 (_s2) {
  }
};


/** Bus stop destination. */
class bus_stop_destination : public bus_stop {
  friend class boost::serialization::access;

  std::string name;

  /** Describe */
  virtual std::string description() const {
    return name;
  }

  /** Serialize. */
  template<class Archive>
  void serialize (Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<bus_stop> (*this) & name;
  }
  
public:
  /** Constructor. */
  bus_stop_destination() {}
  bus_stop_destination (const gps_position& _lat, const gps_position& _long,
                        const std::string& _name)
    : bus_stop(_lat, _long), name(_name) {
  }
};


/////////////////////////////////////////////////////////////
// a bus route is a collection of bus stops
//
// illustrates serialization of STL collection templates.
//
// illustrates serialzation of polymorphic pointer (bus stop *);
//
// illustrates storage and recovery of shared pointers is correct
// and efficient.  That is objects pointed to by more than one
// pointer are stored only once.  In such cases only one such
// object is restored and pointers are restored to point to it
//
class bus_route {
  friend class boost::serialization::access;
  friend std::ostream& operator<< (std::ostream& os, const bus_route& br);

  typedef bus_stop* bus_stop_pointer;
  std::list<bus_stop*> stops;

  /** Serialize */
  template<class Archive>
  void serialize (Archive& ar, const unsigned int version) {
    // In this program, these classes are never serialized directly but rather
    // through a pointer to the base class bus_stop. So we need a way to be
    // sure that the archive contains information about these derived classes.
    //ar.template register_type<bus_stop_corner>();
    ar.register_type (static_cast<bus_stop_corner*>(NULL));
    
    //ar.template register_type<bus_stop_destination>();
    ar.register_type (static_cast<bus_stop_destination*>(NULL));
    // serialization of STL collections is already defined
    // in the header
    ar & stops;
  }
  
public:
  /** Constructor. */
  bus_route() {}

  /** Append a bus stop to the route. */
  void append (BusStopPtr_T _bs) {
    stops.insert (stops.end(), _bs.get());
  }
};

/** Dump into an output stream */
std::ostream& operator<< (std::ostream& os, const bus_route& br) {
  
  // Note: we're displaying the pointer to permit verification
  // that duplicated pointers are properly restored.
  std::list<bus_stop*>::const_iterator it;
  for (it = br.stops.begin(); it != br.stops.end(); it++) {
    os << std::endl << std::hex << "0x" << *it << std::dec << ' ' << **it;
  }
  
  return os;
}

/////////////////////////////////////////////////////////////
// a bus schedule is a collection of routes each with a starting time
//
// Illustrates serialization of STL objects(pair) in a non-intrusive way.
// See definition of operator<< <pair<F, S> >(ar, pair) and others in
// serialization.hpp
// 
// illustrates nesting of serializable classes
//
// illustrates use of version number to automatically grandfather older
// versions of the same class.

class bus_schedule {
public:
  // note: this structure was made public. because the friend declarations
  // didn't seem to work as expected.
  struct trip_info {

    /** Serialize */
    template<class Archive>
    void serialize (Archive& ar, const unsigned int file_version) {
      // In versions 2 or later
      if (file_version >= 2) {
        // read the drivers name
        ar & driver;
      }

      // All versions have the follwing info
      ar & hour & minute;
    }

    // Starting time
    int hour;
    int minute;
    
    // only after system shipped was the driver's name added to the class
    std::string driver;

    /** Constructor. */
    trip_info() {}
    trip_info (int _h, int _m, const std::string& _d)
      : hour(_h), minute(_m), driver(_d) {
    }
  };
  
private:
  friend class boost::serialization::access;
  friend std::ostream & operator<< (std::ostream& os, const bus_schedule& bs);
  friend std::ostream & operator<< (std::ostream& os,
                                    const bus_schedule::trip_info& ti);
  
  std::list<std::pair<trip_info, bus_route*> > schedule;

  /** Serialize. */
  template<class Archive>
  void serialize (Archive& ar, const unsigned int version) {
    ar & schedule;
  }
  
public:
  void append (const std::string& _d, int _h, int _m, bus_route* _br) {
    schedule.insert (schedule.end(),
                     std::make_pair (trip_info(_h, _m, _d), _br));
  }
  
  bus_schedule() {}
};

// ///////////////////////////////////////////////////
BOOST_CLASS_VERSION (bus_schedule, 2)


// ////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const bus_schedule::trip_info& ti) {
  return os << std::endl << ti.hour << ':' << ti.minute << ' ' <<
    ti.driver << ' ';
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const bus_schedule& bs) {
  std::list<std::pair<bus_schedule::trip_info, bus_route *> >::const_iterator it;
  for (it = bs.schedule.begin(); it != bs.schedule.end(); it++) {
    os << it->first << *(it->second);
  }
  return os;
}

// //////////////////////////////////////////////////////////////////////////
void save_schedule (const bus_schedule& s, const char* filename) {
  // Make an archive
  std::ofstream ofs (filename);
  boost::archive::text_oarchive oa (ofs);
  oa << s;
}

// ///////////////////////////////////////////////////////////////////////////
void restore_schedule (bus_schedule& s, const char* filename) {
  // Open the archive
  std::ifstream ifs (filename);
  boost::archive::text_iarchive ia (ifs);

  // Restore the schedule from the archive
  ia >> s;
}



// /////////////////////////////// M A I N ////////////////////////////////////
int main (int argc, char *argv[]) {

  // Make the schedule
  bus_schedule original_schedule;

  // Fill in the data
  // Make a few stops
  BusStopPtr_T bs0 =
    BusStopPtr_T (new bus_stop_corner (gps_position(34, 135, 52.560f),
                                       gps_position(134, 22, 78.30f),
                                       "24th Street", "10th Avenue"));
  BusStopPtr_T bs1 =
    BusStopPtr_T (new bus_stop_corner (gps_position(35, 137, 23.456f),
                                       gps_position(133, 35, 54.12f),
                                       "State street", "Cathedral Vista Lane"));
  BusStopPtr_T bs2 =
    BusStopPtr_T (new bus_stop_destination (gps_position(35, 136, 15.456f),
                                            gps_position(133, 32, 15.300f),
                                            "White House"));
  BusStopPtr_T bs3 =
    BusStopPtr_T (new bus_stop_destination (gps_position(35, 134, 48.789f),
                                            gps_position(133, 32, 16.230f),
                                            "Lincoln Memorial"));

  // Make a few routes
  bus_route route0;
  route0.append (bs0);
  route0.append (bs1);
  route0.append (bs2);

  // Add trips to schedule
  original_schedule.append ("bob", 6, 24, &route0);
  original_schedule.append ("bob", 9, 57, &route0);
  original_schedule.append ("alice", 11, 02, &route0);

  // Make another routes
  bus_route route1;
  route1.append (bs3);
  route1.append (bs2);
  route1.append (bs1);

  // Add trips to schedule
  original_schedule.append ("ted", 7, 17, &route1);
  original_schedule.append ("ted", 9, 38, &route1);
  original_schedule.append ("alice", 11, 47, &route1);

  // Display the complete schedule
  std::cout << "original schedule";
  std::cout << original_schedule;
  std::cout << std::endl;
    
  std::string filename (boost::archive::tmpdir());
  filename += "/serial1.txt";

  // Save the schedule
  save_schedule (original_schedule, filename.c_str());

  // ... some time later
  // Make a new schedule
  bus_schedule new_schedule;

  restore_schedule (new_schedule, filename.c_str());

  // And display
  std::cout << "\nrestored schedule";
  std::cout << new_schedule;
  std::cout << std::endl;
  // Should be the same as the old one (except for the pointer values)

  return 0;
}

