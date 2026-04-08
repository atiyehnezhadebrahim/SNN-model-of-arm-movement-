// #define DEBUG 1
/*
 *  hh_psc_alpha2_nestml.cpp
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Generated from NESTML at time: 2024-12-25 08:10:29.117845
**/

// C++ includes:
#include <limits>

// Includes from libnestutil:
#include "numerics.h"

// Includes from nestkernel:
#include "exceptions.h"
#include "kernel_manager.h"
#include "nest_impl.h"
#include "universal_data_logger_impl.h"

// Includes from sli:
#include "dict.h"
#include "dictutils.h"
#include "doubledatum.h"
#include "integerdatum.h"
#include "lockptrdatum.h"

#include "hh_psc_alpha2_nestml.h"

// ---------------------------------------------------------------------------
//   Recordables map
// ---------------------------------------------------------------------------
nest::RecordablesMap<hh_psc_alpha2_nestml> hh_psc_alpha2_nestml::recordablesMap_;
namespace nest
{

  // Override the create() method with one call to RecordablesMap::insert_()
  // for each quantity to be recorded.
template <> void RecordablesMap<hh_psc_alpha2_nestml>::create()
  {
    // add state variables to recordables map
   insert_(hh_psc_alpha2_nestml_names::_V_m, &hh_psc_alpha2_nestml::get_V_m);
   insert_(hh_psc_alpha2_nestml_names::_Act_m, &hh_psc_alpha2_nestml::get_Act_m);
   insert_(hh_psc_alpha2_nestml_names::_Inact_h, &hh_psc_alpha2_nestml::get_Inact_h);
   insert_(hh_psc_alpha2_nestml_names::_Act_n, &hh_psc_alpha2_nestml::get_Act_n);
   insert_(hh_psc_alpha2_nestml_names::_Actm_NaP, &hh_psc_alpha2_nestml::get_Actm_NaP);
   insert_(hh_psc_alpha2_nestml_names::_Inacth_NaP, &hh_psc_alpha2_nestml::get_Inacth_NaP);
   insert_(hh_psc_alpha2_nestml_names::_K_syn_inh__X__inh_spikes, &hh_psc_alpha2_nestml::get_K_syn_inh__X__inh_spikes);
   insert_(hh_psc_alpha2_nestml_names::_K_syn_inh__X__inh_spikes__d, &hh_psc_alpha2_nestml::get_K_syn_inh__X__inh_spikes__d);
   insert_(hh_psc_alpha2_nestml_names::_K_syn_exc__X__exc_spikes, &hh_psc_alpha2_nestml::get_K_syn_exc__X__exc_spikes);
   insert_(hh_psc_alpha2_nestml_names::_K_syn_exc__X__exc_spikes__d, &hh_psc_alpha2_nestml::get_K_syn_exc__X__exc_spikes__d);
    // add recordable inline expressions to recordables map
	insert_(hh_psc_alpha2_nestml_names::_I_syn_exc, &hh_psc_alpha2_nestml::get_I_syn_exc);
	insert_(hh_psc_alpha2_nestml_names::_I_syn_inh, &hh_psc_alpha2_nestml::get_I_syn_inh);

    // Add vector variables  
  }
}
std::vector< std::tuple< int, int > > hh_psc_alpha2_nestml::rport_to_nestml_buffer_idx =
{
  
  { hh_psc_alpha2_nestml::EXC_SPIKES, hh_psc_alpha2_nestml::INH_SPIKES },
};

// ---------------------------------------------------------------------------
//   Default constructors defining default parameters and state
//   Note: the implementation is empty. The initialization is of variables
//   is a part of hh_psc_alpha2_nestml's constructor.
// ---------------------------------------------------------------------------

hh_psc_alpha2_nestml::Parameters_::Parameters_()
{
}

hh_psc_alpha2_nestml::State_::State_()
{
}

// ---------------------------------------------------------------------------
//   Parameter and state extractions and manipulation functions
// ---------------------------------------------------------------------------

hh_psc_alpha2_nestml::Buffers_::Buffers_(hh_psc_alpha2_nestml &n):
  logger_(n)
  , spike_inputs_( std::vector< nest::RingBuffer >( NUM_SPIKE_RECEPTORS ) )
  , spike_inputs_grid_sum_( std::vector< double >( NUM_SPIKE_RECEPTORS ) )
  , __s( nullptr ), __c( nullptr ), __e( nullptr )
{
  // Initialization of the remaining members is deferred to init_buffers_().
}

hh_psc_alpha2_nestml::Buffers_::Buffers_(const Buffers_ &, hh_psc_alpha2_nestml &n):
  logger_(n)
  , spike_inputs_( std::vector< nest::RingBuffer >( NUM_SPIKE_RECEPTORS ) )
  , spike_inputs_grid_sum_( std::vector< double >( NUM_SPIKE_RECEPTORS ) )
  , __s( nullptr ), __c( nullptr ), __e( nullptr )
{
  // Initialization of the remaining members is deferred to init_buffers_().
}

// ---------------------------------------------------------------------------
//   Default constructor for node
// ---------------------------------------------------------------------------

hh_psc_alpha2_nestml::hh_psc_alpha2_nestml():ArchivingNode(), P_(), S_(), B_(*this)
{
  init_state_internal_();
  recordablesMap_.create();
  pre_run_hook();
}

// ---------------------------------------------------------------------------
//   Copy constructor for node
// ---------------------------------------------------------------------------

hh_psc_alpha2_nestml::hh_psc_alpha2_nestml(const hh_psc_alpha2_nestml& __n):
  ArchivingNode(), P_(__n.P_), S_(__n.S_), B_(__n.B_, *this) {

  // copy parameter struct P_
  P_.t_ref = __n.P_.t_ref;
  P_.g_Na = __n.P_.g_Na;
  P_.g_K = __n.P_.g_K;
  P_.g_L = __n.P_.g_L;
  P_.g_NaP = __n.P_.g_NaP;
  P_.C_m = __n.P_.C_m;
  P_.E_Na = __n.P_.E_Na;
  P_.E_K = __n.P_.E_K;
  P_.E_L = __n.P_.E_L;
  P_.tau_syn_exc = __n.P_.tau_syn_exc;
  P_.E_exc = __n.P_.E_exc;
  P_.tau_syn_inh = __n.P_.tau_syn_inh;
  P_.E_inh = __n.P_.E_inh;
  P_.V_m_init = __n.P_.V_m_init;
  P_.alpha_n_init = __n.P_.alpha_n_init;
  P_.beta_n_init = __n.P_.beta_n_init;
  P_.alpha_m_init = __n.P_.alpha_m_init;
  P_.beta_m_init = __n.P_.beta_m_init;
  P_.alpha_h_init = __n.P_.alpha_h_init;
  P_.beta_h_init = __n.P_.beta_h_init;
  P_.I_e = __n.P_.I_e;

  // copy state struct S_
  S_.r = __n.S_.r;
  S_.ode_state[State_::V_m] = __n.S_.ode_state[State_::V_m];
  S_.ode_state[State_::Act_m] = __n.S_.ode_state[State_::Act_m];
  S_.ode_state[State_::Inact_h] = __n.S_.ode_state[State_::Inact_h];
  S_.ode_state[State_::Act_n] = __n.S_.ode_state[State_::Act_n];
  S_.ode_state[State_::Actm_NaP] = __n.S_.ode_state[State_::Actm_NaP];
  S_.ode_state[State_::Inacth_NaP] = __n.S_.ode_state[State_::Inacth_NaP];
  S_.ode_state[State_::K_syn_inh__X__inh_spikes] = __n.S_.ode_state[State_::K_syn_inh__X__inh_spikes];
  S_.ode_state[State_::K_syn_inh__X__inh_spikes__d] = __n.S_.ode_state[State_::K_syn_inh__X__inh_spikes__d];
  S_.ode_state[State_::K_syn_exc__X__exc_spikes] = __n.S_.ode_state[State_::K_syn_exc__X__exc_spikes];
  S_.ode_state[State_::K_syn_exc__X__exc_spikes__d] = __n.S_.ode_state[State_::K_syn_exc__X__exc_spikes__d];

  // copy internals V_
  V_.RefractoryCounts = __n.V_.RefractoryCounts;
  V_.__h = __n.V_.__h;
  V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes = __n.V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes;
  V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d = __n.V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d;
  V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes = __n.V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes;
  V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d = __n.V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d;
  V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes = __n.V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes;
  V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d = __n.V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d;
  V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes = __n.V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes;
  V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d = __n.V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d;
}

// ---------------------------------------------------------------------------
//   Destructor for node
// ---------------------------------------------------------------------------

hh_psc_alpha2_nestml::~hh_psc_alpha2_nestml()
{
  // GSL structs may not have been allocated, so we need to protect destruction

  if (B_.__s)
  {
    gsl_odeiv_step_free( B_.__s );
  }

  if (B_.__c)
  {
    gsl_odeiv_control_free( B_.__c );
  }

  if (B_.__e)
  {
    gsl_odeiv_evolve_free( B_.__e );
  }
}

// ---------------------------------------------------------------------------
//   Node initialization functions
// ---------------------------------------------------------------------------
void hh_psc_alpha2_nestml::calibrate_time( const nest::TimeConverter& tc )
{
  LOG( nest::M_WARNING,
    "hh_psc_alpha2_nestml",
    "Simulation resolution has changed. Internal state and parameters of the model have been reset!" );

  init_state_internal_();
}
void hh_psc_alpha2_nestml::init_state_internal_()
{
#ifdef DEBUG
  std::cout << "hh_psc_alpha2_nestml::init_state_internal_()" << std::endl;
#endif

  const double __resolution = nest::Time::get_resolution().get_ms();  // do not remove, this is necessary for the resolution() function

  // use a default "good enough" value for the absolute error. It can be adjusted via `node.set()`
  P_.__gsl_error_tol = 1e-3;
  // initial values for parameters
    

    P_.t_ref = 2; // as ms
    

    P_.g_Na = 12000; // as nS
    

    P_.g_K = 3600; // as nS
    

    P_.g_L = 30; // as nS
    

    P_.g_NaP = 300; // as nS
    

    P_.C_m = 100; // as pF
    

    P_.E_Na = 50; // as mV
    

    P_.E_K = (-77); // as mV
    

    P_.E_L = (-54.402); // as mV
    

    P_.tau_syn_exc = 0.2; // as ms
    

    P_.E_exc = 0; // as mV
    

    P_.tau_syn_inh = 2; // as ms
    

    P_.E_inh = (-85.0); // as mV
    

    P_.V_m_init = (-65); // as mV
    

    P_.alpha_n_init = (0.01 * (P_.V_m_init / 1.0 + 55.0)) / (1.0 - std::exp((-(P_.V_m_init / 1.0 + 55.0)) / 10.0)); // as real
    

    P_.beta_n_init = 0.125 * std::exp((-(P_.V_m_init / 1.0 + 65.0)) / 80.0); // as real
    

    P_.alpha_m_init = (0.1 * (P_.V_m_init / 1.0 + 40.0)) / (1.0 - std::exp((-(P_.V_m_init / 1.0 + 40.0)) / 10.0)); // as real
    

    P_.beta_m_init = 4.0 * std::exp((-(P_.V_m_init / 1.0 + 65.0)) / 18.0); // as real
    

    P_.alpha_h_init = 0.07 * std::exp((-(P_.V_m_init / 1.0 + 65.0)) / 20.0); // as real
    

    P_.beta_h_init = 1.0 / (1.0 + std::exp((-(P_.V_m_init / 1.0 + 35.0)) / 10.0)); // as real
    

    P_.I_e = 0; // as pA

  recompute_internal_variables();
  // initial values for state variables
    

    S_.r = 0; // as integer
    

    S_.ode_state[State_::V_m] = P_.V_m_init; // as mV
    

    S_.ode_state[State_::Act_m] = P_.alpha_m_init / (P_.alpha_m_init + P_.beta_m_init); // as real
    

    S_.ode_state[State_::Inact_h] = P_.alpha_h_init / (P_.alpha_h_init + P_.beta_h_init); // as real
    

    S_.ode_state[State_::Act_n] = P_.alpha_n_init / (P_.alpha_n_init + P_.beta_n_init); // as real
    

    S_.ode_state[State_::Actm_NaP] = m_inf(P_.V_m_init); // as real
    

    S_.ode_state[State_::Inacth_NaP] = h_inf(P_.V_m_init); // as real
    

    S_.ode_state[State_::K_syn_inh__X__inh_spikes] = 0; // as real
    

    S_.ode_state[State_::K_syn_inh__X__inh_spikes__d] = 0 * pow(1000.0, (-1)); // as 1 / s
    

    S_.ode_state[State_::K_syn_exc__X__exc_spikes] = 0; // as real
    

    S_.ode_state[State_::K_syn_exc__X__exc_spikes__d] = 0 * pow(1000.0, (-1)); // as 1 / s
}

void hh_psc_alpha2_nestml::init_buffers_()
{
#ifdef DEBUG
  std::cout << "hh_psc_alpha2_nestml::init_buffers_()" << std::endl;
#endif
  // spike input buffers
  get_spike_inputs_().clear();
  get_spike_inputs_grid_sum_().clear();

  // continuous time input buffers  

  get_I_stim().clear();
  B_.I_stim_grid_sum_ = 0;

  B_.logger_.reset();



  if ( not B_.__s )
  {
    B_.__s = gsl_odeiv_step_alloc( gsl_odeiv_step_rkf45, State_::STATE_VEC_SIZE );
  }
  else
  {
    gsl_odeiv_step_reset( B_.__s );
  }

  if ( not B_.__c )
  {
    B_.__c = gsl_odeiv_control_y_new( P_.__gsl_error_tol, 0.0 );
  }
  else
  {
    gsl_odeiv_control_init( B_.__c, P_.__gsl_error_tol, 0.0, 1.0, 0.0 );
  }

  if ( not B_.__e )
  {
    B_.__e = gsl_odeiv_evolve_alloc( State_::STATE_VEC_SIZE );
  }
  else
  {
    gsl_odeiv_evolve_reset( B_.__e );
  }

  B_.__sys.function = hh_psc_alpha2_nestml_dynamics;
  B_.__sys.jacobian = nullptr;
  B_.__sys.dimension = State_::STATE_VEC_SIZE;
  B_.__sys.params = reinterpret_cast< void* >( this );
  B_.__step = nest::Time::get_resolution().get_ms();
  B_.__integration_step = nest::Time::get_resolution().get_ms();
}

void hh_psc_alpha2_nestml::recompute_internal_variables(bool exclude_timestep) {
  const double __resolution = nest::Time::get_resolution().get_ms();  // do not remove, this is necessary for the resolution() function

  if (exclude_timestep) {    
      

      V_.RefractoryCounts = nest::Time(nest::Time::ms((double) (P_.t_ref))).get_steps(); // as integer
      

      V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes = 1.0 * (V_.__h + P_.tau_syn_inh) * std::exp((-V_.__h) / P_.tau_syn_inh) / P_.tau_syn_inh; // as real
      

      V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d = 1.0 * V_.__h * std::exp((-V_.__h) / P_.tau_syn_inh); // as real
      

      V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes = (-1.0) * V_.__h * std::exp((-V_.__h) / P_.tau_syn_inh) / pow(P_.tau_syn_inh, 2); // as real
      

      V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d = 1.0 * ((-V_.__h) + P_.tau_syn_inh) * std::exp((-V_.__h) / P_.tau_syn_inh) / P_.tau_syn_inh; // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes = 1.0 * (V_.__h + P_.tau_syn_exc) * std::exp((-V_.__h) / P_.tau_syn_exc) / P_.tau_syn_exc; // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d = 1.0 * V_.__h * std::exp((-V_.__h) / P_.tau_syn_exc); // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes = (-1.0) * V_.__h * std::exp((-V_.__h) / P_.tau_syn_exc) / pow(P_.tau_syn_exc, 2); // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d = 1.0 * ((-V_.__h) + P_.tau_syn_exc) * std::exp((-V_.__h) / P_.tau_syn_exc) / P_.tau_syn_exc; // as real
  }
  else {    
      

      V_.RefractoryCounts = nest::Time(nest::Time::ms((double) (P_.t_ref))).get_steps(); // as integer
      

      V_.__h = __resolution; // as ms
      

      V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes = 1.0 * (V_.__h + P_.tau_syn_inh) * std::exp((-V_.__h) / P_.tau_syn_inh) / P_.tau_syn_inh; // as real
      

      V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d = 1.0 * V_.__h * std::exp((-V_.__h) / P_.tau_syn_inh); // as real
      

      V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes = (-1.0) * V_.__h * std::exp((-V_.__h) / P_.tau_syn_inh) / pow(P_.tau_syn_inh, 2); // as real
      

      V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d = 1.0 * ((-V_.__h) + P_.tau_syn_inh) * std::exp((-V_.__h) / P_.tau_syn_inh) / P_.tau_syn_inh; // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes = 1.0 * (V_.__h + P_.tau_syn_exc) * std::exp((-V_.__h) / P_.tau_syn_exc) / P_.tau_syn_exc; // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d = 1.0 * V_.__h * std::exp((-V_.__h) / P_.tau_syn_exc); // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes = (-1.0) * V_.__h * std::exp((-V_.__h) / P_.tau_syn_exc) / pow(P_.tau_syn_exc, 2); // as real
      

      V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d = 1.0 * ((-V_.__h) + P_.tau_syn_exc) * std::exp((-V_.__h) / P_.tau_syn_exc) / P_.tau_syn_exc; // as real
  }
}
void hh_psc_alpha2_nestml::pre_run_hook() {
  B_.logger_.init();

  // parameters might have changed -- recompute internals
  recompute_internal_variables();

  // buffers B_
  B_.spike_inputs_.resize(NUM_SPIKE_RECEPTORS);
  B_.spike_inputs_grid_sum_.resize(NUM_SPIKE_RECEPTORS);
}

// ---------------------------------------------------------------------------
//   Functions defined in the NESTML model
// ---------------------------------------------------------------------------

//
double hh_psc_alpha2_nestml::m_inf ( double V_m) const
{  
  return 1.0 / (1.0 + std::exp((-(V_m / 1.0 + 47.1)) / 3.1));
}

//
double hh_psc_alpha2_nestml::Tau_m ( double V_m) const
{  
  return (0.9 * 1.0) / std::cosh((V_m / 1.0 + 47.1) / 6.2);
}

//
double hh_psc_alpha2_nestml::h_inf ( double V_m) const
{  
  return 1.0 / (1.0 + std::exp(V_m / 1.0 + 57.0) / 3.0);
}

//
double hh_psc_alpha2_nestml::Tau_h ( double V_m) const
{  
  return (1200 * 1.0) / std::cosh((V_m / 1.0 + 57.0) / 6.0);
}

// ---------------------------------------------------------------------------
//   Update and spike handling functions
// ---------------------------------------------------------------------------

extern "C" inline int hh_psc_alpha2_nestml_dynamics(double __time, const double ode_state[], double f[], void* pnode)
{
  typedef hh_psc_alpha2_nestml::State_ State_;
  // get access to node so we can almost work as in a member function
  assert( pnode );
  const hh_psc_alpha2_nestml& node = *( reinterpret_cast< hh_psc_alpha2_nestml* >( pnode ) );

  // ode_state[] here is---and must be---the state vector supplied by the integrator,
  // not the state vector in the node, node.S_.ode_state[].
  f[State_::Act_n] = (-0.01) * ode_state[State_::Act_n] * ode_state[State_::V_m] / (1.0 - 0.00408677143846407 * std::exp((-0.1) * ode_state[State_::V_m])) - 0.055468413760135 * ode_state[State_::Act_n] * std::exp((-0.0125) * ode_state[State_::V_m]) - 0.55 * ode_state[State_::Act_n] / (1.0 - 0.00408677143846407 * std::exp((-0.1) * ode_state[State_::V_m])) + 0.01 * ode_state[State_::V_m] / (1.0 - 0.00408677143846407 * std::exp((-0.1) * ode_state[State_::V_m])) + 0.55 / (1.0 - 0.00408677143846407 * std::exp((-0.1) * ode_state[State_::V_m]));
  f[State_::Act_m] = (-0.1) * ode_state[State_::Act_m] * ode_state[State_::V_m] / (1.0 - 0.0183156388887342 * std::exp((-0.1) * ode_state[State_::V_m])) - 0.108087223804836 * ode_state[State_::Act_m] * std::exp((-0.0555555555555556) * ode_state[State_::V_m]) - 4.0 * ode_state[State_::Act_m] / (1.0 - 0.0183156388887342 * std::exp((-0.1) * ode_state[State_::V_m])) + 0.1 * ode_state[State_::V_m] / (1.0 - 0.0183156388887342 * std::exp((-0.1) * ode_state[State_::V_m])) + 4.0 / (1.0 - 0.0183156388887342 * std::exp((-0.1) * ode_state[State_::V_m]));
  f[State_::Inact_h] = (-0.00271419454822054) * ode_state[State_::Inact_h] * std::exp((-0.05) * ode_state[State_::V_m]) - 1.0 * ode_state[State_::Inact_h] / (0.0301973834223185 * std::exp((-0.1) * ode_state[State_::V_m]) + 1.0) + 0.00271419454822054 * std::exp((-0.05) * ode_state[State_::V_m]);
  f[State_::Actm_NaP] = (-ode_state[State_::Actm_NaP]) / node.Tau_m(ode_state[State_::V_m]) + node.m_inf(ode_state[State_::V_m]) / node.Tau_m(ode_state[State_::V_m]);
  f[State_::Inacth_NaP] = (-ode_state[State_::Inacth_NaP]) / node.Tau_h(ode_state[State_::V_m]) + node.h_inf(ode_state[State_::V_m]) / node.Tau_h(ode_state[State_::V_m]);
  f[State_::V_m] = node.P_.E_Na * (pow(ode_state[State_::Act_m], 3) * ode_state[State_::Inact_h] * node.P_.g_Na / node.P_.C_m + ode_state[State_::Actm_NaP] * ode_state[State_::Inacth_NaP] * node.P_.g_NaP / node.P_.C_m) + node.P_.g_K * (pow(ode_state[State_::Act_n], 4) * node.P_.E_K / node.P_.C_m - pow(ode_state[State_::Act_n], 4) * ode_state[State_::V_m] / node.P_.C_m) + node.P_.g_L * (node.P_.E_L / node.P_.C_m - ode_state[State_::V_m] / node.P_.C_m) - 1.0 * node.P_.E_exc * ode_state[State_::K_syn_exc__X__exc_spikes] / node.P_.C_m + 1.0 * node.P_.E_inh * ode_state[State_::K_syn_inh__X__inh_spikes] / node.P_.C_m + node.P_.I_e / node.P_.C_m + ((-pow(ode_state[State_::Act_m], 3)) * ode_state[State_::Inact_h] * ode_state[State_::V_m] * node.P_.g_Na - ode_state[State_::Actm_NaP] * ode_state[State_::Inacth_NaP] * ode_state[State_::V_m] * node.P_.g_NaP + node.B_.I_stim_grid_sum_ + 1.0 * ode_state[State_::K_syn_exc__X__exc_spikes] * ode_state[State_::V_m] - 1.0 * ode_state[State_::K_syn_inh__X__inh_spikes] * ode_state[State_::V_m]) / node.P_.C_m;
  f[State_::K_syn_inh__X__inh_spikes] = 1.0 * ode_state[State_::K_syn_inh__X__inh_spikes__d];
  f[State_::K_syn_inh__X__inh_spikes__d] = (-ode_state[State_::K_syn_inh__X__inh_spikes]) / pow(node.P_.tau_syn_inh, 2) - 2 * ode_state[State_::K_syn_inh__X__inh_spikes__d] / node.P_.tau_syn_inh;
  f[State_::K_syn_exc__X__exc_spikes] = 1.0 * ode_state[State_::K_syn_exc__X__exc_spikes__d];
  f[State_::K_syn_exc__X__exc_spikes__d] = (-ode_state[State_::K_syn_exc__X__exc_spikes]) / pow(node.P_.tau_syn_exc, 2) - 2 * ode_state[State_::K_syn_exc__X__exc_spikes__d] / node.P_.tau_syn_exc;
  return GSL_SUCCESS;
}

void hh_psc_alpha2_nestml::update(nest::Time const & origin,const long from, const long to)
{
  const double __resolution = nest::Time::get_resolution().get_ms();  // do not remove, this is necessary for the resolution() function



  for ( long lag = from ; lag < to ; ++lag )
  {
    auto get_t = [origin, lag](){ return nest::Time( nest::Time::step( origin.get_steps() + lag + 1) ).get_ms(); };

    for (long i = 0; i < NUM_SPIKE_RECEPTORS; ++i)
    {
        get_spike_inputs_grid_sum_()[i] = get_spike_inputs_()[i].get_value(lag);
    }
    B_.I_stim_grid_sum_ = get_I_stim().get_value(lag);

    // NESTML generated code for the update block
  double U_old = S_.ode_state[State_::V_m];
  double K_syn_inh__X__inh_spikes__tmp = S_.ode_state[State_::K_syn_inh__X__inh_spikes] * V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes + S_.ode_state[State_::K_syn_inh__X__inh_spikes__d] * V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d;
  double K_syn_inh__X__inh_spikes__d__tmp = S_.ode_state[State_::K_syn_inh__X__inh_spikes] * V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes + S_.ode_state[State_::K_syn_inh__X__inh_spikes__d] * V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d;
  double K_syn_exc__X__exc_spikes__tmp = S_.ode_state[State_::K_syn_exc__X__exc_spikes] * V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes + S_.ode_state[State_::K_syn_exc__X__exc_spikes__d] * V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d;
  double K_syn_exc__X__exc_spikes__d__tmp = S_.ode_state[State_::K_syn_exc__X__exc_spikes] * V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes + S_.ode_state[State_::K_syn_exc__X__exc_spikes__d] * V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d;
  double __t = 0;
  // numerical integration with adaptive step size control:
  // ------------------------------------------------------
  // gsl_odeiv_evolve_apply performs only a single numerical
  // integration step, starting from t and bounded by step;
  // the while-loop ensures integration over the whole simulation
  // step (0, step] if more than one integration step is needed due
  // to a small integration step size;
  // note that (t+IntegrationStep > step) leads to integration over
  // (t, step] and afterwards setting t to step, but it does not
  // enforce setting IntegrationStep to step-t; this is of advantage
  // for a consistent and efficient integration across subsequent
  // simulation intervals
  while ( __t < B_.__step )
  {

    const int status = gsl_odeiv_evolve_apply(B_.__e,
                                              B_.__c,
                                              B_.__s,
                                              &B_.__sys,              // system of ODE
                                              &__t,                   // from t
                                              B_.__step,              // to t <= step
                                              &B_.__integration_step, // integration step size
                                              S_.ode_state);          // neuronal state

    if ( status != GSL_SUCCESS )
    {
      throw nest::GSLSolverFailure( get_name(), status );
    }
  }
  /* replace analytically solvable variables with precisely integrated values  */
  S_.ode_state[State_::K_syn_inh__X__inh_spikes] = K_syn_inh__X__inh_spikes__tmp;
  S_.ode_state[State_::K_syn_inh__X__inh_spikes__d] = K_syn_inh__X__inh_spikes__d__tmp;
  S_.ode_state[State_::K_syn_exc__X__exc_spikes] = K_syn_exc__X__exc_spikes__tmp;
  S_.ode_state[State_::K_syn_exc__X__exc_spikes__d] = K_syn_exc__X__exc_spikes__d__tmp;
  S_.ode_state[State_::K_syn_inh__X__inh_spikes__d] += ((0.001 * B_.spike_inputs_grid_sum_[INH_SPIKES - MIN_SPIKE_RECEPTOR])) * (numerics::e / P_.tau_syn_inh) / (1 / 1000.0);
  S_.ode_state[State_::K_syn_exc__X__exc_spikes__d] += ((0.001 * B_.spike_inputs_grid_sum_[EXC_SPIKES - MIN_SPIKE_RECEPTOR])) * (numerics::e / P_.tau_syn_exc) / (1 / 1000.0);
  if (S_.r > 0)
  {  
    S_.r -= 1;
  }
  else if (S_.ode_state[State_::V_m] > 0 && U_old > S_.ode_state[State_::V_m])
  {  
    S_.r = V_.RefractoryCounts;
    set_spiketime(nest::Time::step(origin.get_steps()+lag+1));
    nest::SpikeEvent se;
    nest::kernel().event_delivery_manager.send(*this, se, lag);
  }
    // voltage logging
    B_.logger_.record_data(origin.get_steps() + lag);
  }
}

// Do not move this function as inline to h-file. It depends on
// universal_data_logger_impl.h being included here.
void hh_psc_alpha2_nestml::handle(nest::DataLoggingRequest& e)
{
  B_.logger_.handle(e);
}


void hh_psc_alpha2_nestml::handle(nest::SpikeEvent &e)
{
  assert(e.get_delay_steps() > 0);
  assert( e.get_rport() < B_.spike_inputs_.size() );

  double weight = e.get_weight();
  size_t nestml_buffer_idx = 0;
  if ( weight >= 0.0 )
  {
    nestml_buffer_idx = std::get<0>(rport_to_nestml_buffer_idx[e.get_rport()]);
  }
  else
  {
    nestml_buffer_idx = std::get<1>(rport_to_nestml_buffer_idx[e.get_rport()]);
    if ( nestml_buffer_idx == hh_psc_alpha2_nestml::PORT_NOT_AVAILABLE )
    {
      nestml_buffer_idx = std::get<0>(rport_to_nestml_buffer_idx[e.get_rport()]);
    }
    weight = -weight;
  }
  B_.spike_inputs_[ nestml_buffer_idx - MIN_SPIKE_RECEPTOR ].add_value(
    e.get_rel_delivery_steps( nest::kernel().simulation_manager.get_slice_origin() ),
    weight * e.get_multiplicity() );
}

void hh_psc_alpha2_nestml::handle(nest::CurrentEvent& e)
{
  assert(e.get_delay_steps() > 0);

  const double current = e.get_current();     // we assume that in NEST, this returns a current in pA
  const double weight = e.get_weight();
  get_I_stim().add_value(
               e.get_rel_delivery_steps( nest::kernel().simulation_manager.get_slice_origin()),
               weight * current );
}

