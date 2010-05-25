/** Testing brocast funtion in boost::mpi  */

#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>

class Pos{
  private:
    int x;
    int y;
    friend class boost::serialization::access;

    template<class Archive>
      void serialize(Archive & ar, const unsigned int version){
        ar & x ;
        ar & y;
      }

  public:
    Pos()   {  };
    Pos(int x1, int y1): x(x1), y(y1)   {  };
    int getX(); 
    void afficher();
};

using namespace std;

int Pos::getX(){
  return x;
}
void Pos::afficher(){
  cout << "(" << x << ";" << y << ")" << endl;
}

namespace mpi=boost::mpi;

int main(int argc, char ** argv ){
  /*initialisation of mpi*/
  mpi::environment env(argc, argv);

  mpi::communicator world;
  int rank = world.rank();//processus number 

  vector<int> vec;
  Pos pos (100,1000); 

  if (rank==0){
    vec.push_back(20);
    pos = Pos (10,10);
    cout << vec[0] << endl;
  }else  {
    ;
  }
  broadcast (world, vec, 0);//send to everyone value of vec defined in processus 0
  broadcast (world, pos, 0);//the same, but the valeu of pos

  /*testing*/
  pos.afficher();
  cout << "processus " << rank << ":" << vec[0] << endl; 
}

