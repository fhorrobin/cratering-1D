#include "my_main_loop.hpp"
#include "source/newtonian/test_1d/main_loop_1d.hpp"
#include "source/newtonian/one_dimensional/hdf5_diagnostics1d.hpp"
#include "source/misc/int2str.hpp"
#include "source/misc/vector_initialiser.hpp"
#include "source/misc/simple_io.hpp"
#include <fstream>

using simulation1d::main_loop;
using simulation1d::SafeTimeTermination;
using simulation1d::DiagnosticsFunction;
using simulation1d::WriteTime;
using diagnostics1d::write_snapshot_to_hdf5;
using std::pair;
using std::ofstream;
using std::endl;

namespace {
  class MultipleDiagnostics: public DiagnosticsFunction
  {
  public:

    MultipleDiagnostics
    (const vector<DiagnosticsFunction*>& diag_list):
      diag_list_(diag_list) {}

    ~MultipleDiagnostics(void)
    {
      for(size_t i=0;i<diag_list_.size();++i)
	delete diag_list_.at(i);
    }

    void diagnose(const hdsim1D& sim)
    {
      for(size_t i=0;i<diag_list_.size();++i)
	diag_list_.at(i)->diagnose(sim);
    }

  private:
    vector<DiagnosticsFunction*> diag_list_;
  };

  class IntervalSnapshot: public DiagnosticsFunction
  {
  public:

    IntervalSnapshot(double dt):
      counter_(0), t_next_(0), dt_(dt) {}

    void diagnose(const hdsim1D& sim)
    {
      if(sim.GetTime()>t_next_){
	write_snapshot_to_hdf5
	  (sim,
	   "snapshot_"+int2str(counter_)+".h5");
	t_next_ += dt_;
	counter_++;
      }
    }

  private:
    mutable int counter_;
    mutable double t_next_;
    const double dt_;
  };

  class TimeStep: public DiagnosticsFunction
  {
  public:
    TimeStep(const string& fname):
      fname_(fname),
      t_prev_(0) {}

    void diagnose(const hdsim1D& sim)
    {
      const double t_cur = sim.GetTime();
      const double dt = t_cur - t_prev_;
      t_prev_ = t_cur;
      write_number(dt, fname_);
    }

  private:
    const string fname_;
    mutable double t_prev_;
  };

  class WaveArrival: public simulation1d::TerminationCondition
  {
  public:
    WaveArrival(double pos,
		double v_thres):
      pos_(pos),
      v_thres_(v_thres) {}

    bool operator()(const hdsim1D& sim)
    {
      return sameple_velocity(sim,pos_)<v_thres_;
    }

  private:
    const double pos_;
    const double v_thres_;

    double sameple_velocity
    (const hdsim1D& sim,
     double pos)
    {
      const size_t vertex_number = static_cast<size_t>(sim.GetVertexNo());
      size_t i = 0;
      while(sim.GetVertexPosition(i)<pos)
	++i;
      return sim.GetCell(i).Velocity.x;
    }
  };

  class VelocityHistory: public simulation1d::DiagnosticsFunction
  {
  public:

    VelocityHistory(double pos,
		    const string& fname):
      pos_(pos), 
      fname_(fname),
      t_list_(),
      v_list_() {}

    void diagnose(const hdsim1D& sim)
    {
      size_t i = 0;
      while(sim.GetVertexPosition(i)<pos_)
	++i;
      v_list_.push_back(sim.GetCell(i).Velocity.x);
      t_list_.push_back(sim.GetTime());
    }
    
    ~VelocityHistory(void)
    {
      ofstream f(fname_.c_str());
      for(size_t i=0;i<v_list_.size();++i)
	f << t_list_.at(i) << " " << v_list_.at(i) << endl;
      f.close();
    }

  private:
    const double pos_;
    const string fname_;
    mutable vector<double> t_list_;
    mutable vector<double> v_list_;
  };
}

void my_main_loop(hdsim1D& sim)
{
  WaveArrival term_cond(1.5,1e-3);
  MultipleDiagnostics diag
    (VectorInitialiser<DiagnosticsFunction*>
     (new WriteTime("time.txt"))
     (new TimeStep("dt.txt"))
     (new VelocityHistory(0.99,"velocity_history.txt"))
     (new IntervalSnapshot(0.1))());
  main_loop(sim,
	    term_cond,
	    1,
	    &diag);
}
