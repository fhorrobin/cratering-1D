#include "create_grid.hpp"
#include <cmath>
#include <cassert>

namespace{

  double calc_dr(double x, double q, double dr_min)
  {
    return fabs(x)*q+dr_min;
  }

  class StepFunction
  {
  public:

    StepFunction(double q, double dr_min):
      q_(q), dr_min_(dr_min) {}

    double operator()(double x) const
    {
      return calc_dr(x, q_, dr_min_);
    }

  private:
    const double q_;
    const double dr_min_;  
  };
}

vector<double> create_grid
(double left,
 double right,
 double q,
 double dr_min)
{
  assert(right>left);
  const StepFunction sf(q,dr_min);
  vector<double> res;
  for(double x=left;x<right;x+=sf(x))
    res.push_back(x);
  return res;
}
