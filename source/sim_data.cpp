#include "sim_data.hpp"
#include "create_grid.hpp"
#include <cmath>

Conserved Ratchet::CalcFlux(vector<double> const& Vertices,
			    vector<Primitive> const& Cells,
			    RiemannSolver const& rs, 
			    vector<double> const& vertex_velocity,
			    int i) const
{
  if(i==0){
    Primitive ghost = Cells[0];
    //    ghost.Velocity.x = -abs(ghost.Velocity.x);
    ghost.Density = fmax(ghost.Density/2,1e-9);
    ghost.Pressure = fmax(ghost.Pressure/2,1e-9);
    double vv = vertex_velocity[0];
    return rs(ghost,Cells[0],vv);
  }
  else if(i==static_cast<int>(Vertices.size())-1){
    Primitive ghost = Cells[Vertices.size()-2];
    //    ghost.Velocity.x = abs(ghost.Velocity.x);
    ghost.Density = fmax(ghost.Density/2,1e-9);
    ghost.Pressure = fmax(ghost.Pressure/2,1e-9);
    double vv = vertex_velocity[Vertices.size()-1];
    return rs(Cells[Vertices.size()-2],ghost,vv);
  }
  else
    throw UniversalError("Index inside bulk of grid");
}


namespace {
  vector<double> logspace
  (double start,
   double end,
   size_t n)
  {
    vector<double> res(n);
    const double len = static_cast<double>(n);
    for(size_t i=0;i<n;i++){
      const double id = static_cast<double>(i);
      const double expnt = start + (end-start)*id/len;
      res.at(i) = pow(10,expnt);
    }
    return res;
  }
}

// Set variable parameters here
SimData::SimData(double iw):
  pg_(),
  sr_(),
  eos_(9./3.),
  rs_(),
  vm_(),
  bc_(),
  source_(),
  grid_(linspace(-1,2,3e3)),
  sim_(pg_,
       grid_,
       sr_,
       TwoSteps(1e-4,-iw,1,1,1e-4),
       Uniform(1e-9),
       TwoSteps(0,-iw,1,0,0),
       Uniform(0),
       eos_,
       rs_,
       vm_,
       bc_,
       source_)
{
  assert(grid_.size()>1);
}

hdsim1D& SimData::getSim(void)
{
  return sim_;
}
