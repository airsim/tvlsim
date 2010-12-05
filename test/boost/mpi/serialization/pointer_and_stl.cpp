/*
 * testing : serialize pointer and stl collections
 * */
#include <fstream>

// include headers that implement a archive in simple text format
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>

//boost mpi; that include automatically text_oarchive and text_iarchive
#include</usr/include/boost/mpi.hpp>
using namespace std;

/* if a class is empty (it doesn't contain any variable) does it have to be serialized ?
 * Answer:no*/
class empty_class{
  public:
    static void do_nothing(){
      ;
    }
    empty_class(){  };
};


class person{
  friend class boost::serialization::access;
  private:
  int content;
  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & content;
    }

  public:
  typedef vector<person> group_person;
  person(){  };
  person(int c):content(c){  };
  void show(){  cout << "content=" << content << endl; };
};


class nothing{
  friend class boost::serialization::access;
  private:
  int num;
  person *per;
  person::group_person group;
  empty_class empty;

  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & num;
      ar & per;//add pointer: important!!
      ar & group;//add stl container: important!!
    }

  public:
  nothing(){};
  nothing(int a1, int b1, person::group_person p)  {
    num = a1;
    per = new person(b1);
    group = p;
  }
  void show(){
    cout << "num=" << num << endl;
    per -> show();
    for (int i=0; i<group.size(); i++){
      group[i].show();
    };

  };
};


int main(int argc, char ** argv){
  namespace mpi=boost::mpi;
  using namespace std;

  mpi::environment env(argc, argv);
  mpi::communicator world;
  int rank = world.rank();
  int dest = 1, tag=0, src = 0;
  person::group_person group;
  group.push_back(person(10));
  group.push_back(person(20));

  if (rank == 0){
    nothing p (1,2, group) ;
    p.show();
    world.send(dest, tag, p);
  }else{
    nothing q;
    world.recv(src, tag, q);
    q.show();
  };
};


