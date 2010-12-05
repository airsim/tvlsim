// STL
#include <iostream>
#include <algorithm>
// Boost
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/foreach.hpp>
// Boost Accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

// using namespace boost;
namespace boostacc = boost::accumulators;

// Helper that uses BOOST_FOREACH to display a range of doubles
template<typename Range>
void output_range(Range const &rng) {
  bool first = true;
  BOOST_FOREACH(double d, rng) {
    if(!first) std::cout << ", ";
    std::cout << d;
    first = false;
  }
  std::cout << '\n';
}

///////////////////////////////////////////////////////////////////////////////
// example1
//
//  Calculate some useful stats using accumulator_set<> and std::for_each()
//
void example1() {
  boostacc::accumulator_set<double,
    boostacc::stats<boostacc::tag::min,
          boostacc::tag::mean(boostacc::immediate),
          boostacc::tag::sum,
          boostacc::tag::moment<2> > > acc;

  boost::array<double, 4> data = {0., 1., -1., 3.14159};

  // std::for_each pushes each sample into the accumulator one at a
  // time, and returns a copy of the accumulator.
  acc = std::for_each(data.begin(), data.end(), acc);

  // The following would be equivalent, and could be more efficient
  // because it doesn't pass and return the entire accumulator set
  // by value.
  //std::for_each(data.begin(), data.end(), bind<void>(ref(acc), _1));
  
  std::cout << "  min(acc)        = " << (boostacc::min)(acc) << std::endl;
  std::cout << "  mean(acc)       = " << boostacc::mean(acc) << std::endl;
  
  // since mean depends on count and sum, we can get their results, too.
  std::cout << "  count(acc)      = " << boostacc::count(acc) << std::endl;
  std::cout << "  sum(acc)        = " << boostacc::sum(acc) << std::endl;
  std::cout << "  moment<2>(acc)  = " << boostacc::moment<2>(acc) << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// example2
//
//  Calculate some tail statistics. This demonstrates how to specify
//  constructor and accumulator parameters. Note that the tail statistics
//  return multiple values, which are returned in an iterator_range.
//
//  It pushes data in and displays the intermediate results to demonstrate
//  how the tail statistics are updated.
void example2() {
  // An accumulator which tracks the right tail (largest N items) and
  // some data that are covariate with them. N == 4.
  boostacc::accumulator_set<double,
    boostacc::stats<
    boostacc::tag::tail_variate<double,
    boostacc::tag::covariate1, boostacc::right> > > acc (boostacc::tag::tail<boostacc::right>::cache_size = 4);

  acc(2.1, boostacc::covariate1 = .21);
  acc(1.1, boostacc::covariate1 = .11);
  acc(2.1, boostacc::covariate1 = .21);
  acc(1.1, boostacc::covariate1 = .11);

  std::cout << "  tail            = "; output_range(boostacc::tail(acc));
  std::cout << "  tail_variate    = "; output_range(boostacc::tail_variate(acc));
  std::cout << std::endl;
  
  acc(21.1, boostacc::covariate1 = 2.11);
  acc(11.1, boostacc::covariate1 = 1.11);
  acc(21.1, boostacc::covariate1 = 2.11);
  acc(11.1, boostacc::covariate1 = 1.11);
  
  std::cout << "  tail            = "; output_range(boostacc::tail(acc));
  std::cout << "  tail_variate    = "; output_range(boostacc::tail_variate(acc));
  std::cout << std::endl;
  
  acc(42.1, boostacc::covariate1 = 4.21);
  acc(41.1, boostacc::covariate1 = 4.11);
  acc(42.1, boostacc::covariate1 = 4.21);
  acc(41.1, boostacc::covariate1 = 4.11);
  
  std::cout << "  tail            = "; output_range(boostacc::tail(acc));
  std::cout << "  tail_variate    = "; output_range(boostacc::tail_variate(acc));
  std::cout << std::endl;
  
  acc(32.1, boostacc::covariate1 = 3.21);
  acc(31.1, boostacc::covariate1 = 3.11);
  acc(32.1, boostacc::covariate1 = 3.21);
  acc(31.1, boostacc::covariate1 = 3.11);
  
  std::cout << "  tail            = "; output_range(boostacc::tail(acc));
  std::cout << "  tail_variate    = "; output_range(boostacc::tail_variate(acc));
}

///////////////////////////////////////////////////////////////////////////////
// example3
//
//  Demonstrate how to calculate weighted statistics. This example demonstrates
//  both a simple weighted statistical calculation, and a more complicated
//  calculation where the weight statistics are calculated and stored in an
//  external weight accumulataor.
void example3() {
  // weight == double
  double w = 1.;

  // Simple weighted calculation
  {
    // stats that depend on the weight are made external
    boostacc::accumulator_set<double,
      boostacc::stats<boostacc::tag::mean>, double> acc;
    
    acc(0., boostacc::weight = w);
    acc(1., boostacc::weight = w);
    acc(-1., boostacc::weight = w);
    acc(3.14159, boostacc::weight = w);

    std::cout << "  mean(acc)       = " << boostacc::mean(acc) << std::endl;
  }
  
  // Weighted calculation with an external weight accumulator
  {
    // stats that depend on the weight are made external
    boostacc::accumulator_set<double, boostacc::stats<boostacc::tag::mean>,
      boostacc::external<double> > acc;
    
    // Here's an external weight accumulator
    boostacc::accumulator_set<void, boostacc::stats<boostacc::tag::sum_of_weights>, double> weight_acc;
    
    weight_acc(boostacc::weight = w); acc(0., boostacc::weight = w);
    weight_acc(boostacc::weight = w); acc(1., boostacc::weight = w);
    weight_acc(boostacc::weight = w); acc(-1., boostacc::weight = w);
    weight_acc(boostacc::weight = w); acc(3.14159, boostacc::weight = w);
    
    std::cout << "  mean(acc)       = "
              << boostacc::mean(acc, boostacc::weights = weight_acc)
              << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
// main
int main (int argc, char* argv[]) {
  std::cout << "Example 1:" << std::endl;
  example1();
  
  std::cout << std::endl << "Example 2:" << std::endl;
  example2();
  
  std::cout << std::endl << "Example 3:" << std::endl;
  example3();
  
  return 0;
}
