#ifndef SIM_DATA_HPP
#define SIM_DATA_HPP 1

#include "source/newtonian/one_dimensional/physical_geometry_1d.hpp"
#include "source/newtonian/one_dimensional/pcm1d.hpp"
#include "source/newtonian/one_dimensional/hdsim.hpp"
#include "source/newtonian/common/ideal_gas.hpp"
#include "source/newtonian/common/hllc.hpp"
#include "source/newtonian/one_dimensional/eulerian1d.hpp"
#include "source/newtonian/one_dimensional/rigid_wall_1d.hpp"
#include "source/newtonian/one_dimensional/zero_force_1d.hpp"
#include "source/misc/simple_io.hpp"

class Ratchet: public BoundaryConditions1D
{
public:

  Conserved CalcFlux(const vector<double>& vertices,
		     const vector<Primitive>& cells,
		     const RiemannSolver& rs,
		     vector<double> const& vertex_veclocity,
		     int i) const;
};

class SimData
{
public:

  SimData(double iw);

  hdsim1D& getSim(void);

private:
  const SlabSymmetry1D pg_;
  const PCM1D sr_;
  const IdealGas eos_;
  const Hllc rs_;
  const Eulerian1D vm_;
  const Ratchet bc_;
  const ZeroForce1D source_;
  const vector<double> grid_;
  hdsim1D sim_;
};

#endif // SIM_DATA_HPP
