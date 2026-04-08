
/**
 *  hh_psc_alpha2_nestml.h
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
#ifndef HH_PSC_ALPHA2_NESTML
#define HH_PSC_ALPHA2_NESTML

#ifndef HAVE_LIBLTDL
#error "NEST was compiled without support for dynamic loading. Please install libltdl and recompile NEST."
#endif

// C++ includes:
#include <cmath>

#include "config.h"

#ifndef HAVE_GSL
#error "The GSL library is required for the Runge-Kutta solver."
#endif

// External includes:
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

// Includes from nestkernel:
#include "archiving_node.h"
#include "connection.h"
#include "dict_util.h"
#include "event.h"
#include "nest_types.h"
#include "ring_buffer.h"
#include "universal_data_logger.h"

// Includes from sli:
#include "dictdatum.h"

namespace nest
{
namespace hh_psc_alpha2_nestml_names
{
    const Name _r( "r" );
    const Name _V_m( "V_m" );
    const Name _Act_m( "Act_m" );
    const Name _Inact_h( "Inact_h" );
    const Name _Act_n( "Act_n" );
    const Name _Actm_NaP( "Actm_NaP" );
    const Name _Inacth_NaP( "Inacth_NaP" );
    const Name _K_syn_inh__X__inh_spikes( "K_syn_inh__X__inh_spikes" );
    const Name _K_syn_inh__X__inh_spikes__d( "K_syn_inh__X__inh_spikes__d" );
    const Name _K_syn_exc__X__exc_spikes( "K_syn_exc__X__exc_spikes" );
    const Name _K_syn_exc__X__exc_spikes__d( "K_syn_exc__X__exc_spikes__d" );
    const Name _I_syn_exc( "I_syn_exc" );
    const Name _I_syn_inh( "I_syn_inh" );
    const Name _t_ref( "t_ref" );
    const Name _g_Na( "g_Na" );
    const Name _g_K( "g_K" );
    const Name _g_L( "g_L" );
    const Name _g_NaP( "g_NaP" );
    const Name _C_m( "C_m" );
    const Name _E_Na( "E_Na" );
    const Name _E_K( "E_K" );
    const Name _E_L( "E_L" );
    const Name _tau_syn_exc( "tau_syn_exc" );
    const Name _E_exc( "E_exc" );
    const Name _tau_syn_inh( "tau_syn_inh" );
    const Name _E_inh( "E_inh" );
    const Name _V_m_init( "V_m_init" );
    const Name _alpha_n_init( "alpha_n_init" );
    const Name _beta_n_init( "beta_n_init" );
    const Name _alpha_m_init( "alpha_m_init" );
    const Name _beta_m_init( "beta_m_init" );
    const Name _alpha_h_init( "alpha_h_init" );
    const Name _beta_h_init( "beta_h_init" );
    const Name _I_e( "I_e" );
}
}



/**
 * Function computing right-hand side of ODE for GSL solver.
 * @note Must be declared here so we can befriend it in class.
 * @note Must have C-linkage for passing to GSL. Internally, it is
 *       a first-class C++ function, but cannot be a member function
 *       because of the C-linkage.
 * @note No point in declaring it inline, since it is called
 *       through a function pointer.
 * @param void* Pointer to model neuron instance.
**/
extern "C" inline int hh_psc_alpha2_nestml_dynamics( double, const double y[], double f[], void* pnode );


#include "nest_time.h"
  typedef nest::port nest_port_t;
  typedef nest::rport nest_rport_t;

/* BeginDocumentation
  Name: hh_psc_alpha2_nestml

  Description:

    """
  hh_psc_alpha - Hodgkin-Huxley neuron model#########################################

  Description
  +++++++++++

  hh_psc_alpha is an implementation of a spiking neuron using the Hodgkin-Huxley
  formalism.

  Incoming spike events induce a post-synaptic change of current modelled
  by an alpha function. The alpha function is normalised such that an event of
  weight 1.0 results in a peak current of 1 pA.

  Spike detection is done by a combined threshold-and-local-maximum search: if
  there is a local maximum above a certain threshold of the membrane potential,
  it is considered a spike.


  Problems/Todo
  +++++++++++++

  - better spike detection
  - initial wavelet/spike at simulation onset


  References
  ++++++++++

  .. [1] Gerstner W, Kistler W (2002). Spiking neuron models: Single neurons,
         populations, plasticity. New York: Cambridge University Press
  .. [2] Dayan P, Abbott LF (2001). Theoretical neuroscience: Computational and
         mathematical modeling of neural systems. Cambridge, MA: MIT Press.
         https://pure.mpg.de/pubman/faces/ViewItemOverviewPage.jsp?itemId=item_3006127>
  .. [3] Hodgkin AL and Huxley A F (1952). A quantitative description of
         membrane current and its application to conduction and excitation in
         nerve. The Journal of Physiology 117.
         DOI: https://doi.org/10.1113/jphysiol.1952.sp004764


  See also
  ++++++++

  hh_cond_exp_traub
  """


  Parameters:
  The following parameters can be set in the status dictionary.
t_ref [ms]  Refractory period
g_Na [nS]  Sodium peak conductance
g_K [nS]  Potassium peak conductance
g_L [nS]  Leak conductance
g_NaP [nS]  persistent Sodium Conductance
C_m [pF]  Membrane Capacitance
E_Na [mV]  Sodium reversal potential
E_K [mV]  Potassium reversal potential
E_L [mV]  Leak reversal Potential (aka resting potential)
tau_syn_exc [ms]  Rise time of the excitatory synaptic alpha function
E_exc [mV]  Excitatory reversal Potential
tau_syn_inh [ms]  Rise time of the inhibitory synaptic alpha function
E_inh [mV]  Inhibitory reversal Potential
V_m_init [mV]  Initial membrane potential
I_e [pA]  constant external input current


  Dynamic state variables:
r [integer]  number of steps in the current refractory phase
V_m [mV]  Membrane potential
Act_m [real]  Activation variable m for Na
Inact_h [real]  Inactivation variable h for Na
Act_n [real]  Activation variable n for K


  Sends: nest::SpikeEvent

  Receives: Spike, Current, DataLoggingRequest
*/

// Register the neuron model

class hh_psc_alpha2_nestml : public nest::ArchivingNode
{
public:
  /**
   * The constructor is only used to create the model prototype in the model manager.
  **/
  hh_psc_alpha2_nestml();

  /**
   * The copy constructor is used to create model copies and instances of the model.
   * @node The copy constructor needs to initialize the parameters and the state.
   *       Initialization of buffers and interal variables is deferred to
   *       @c init_buffers_() and @c pre_run_hook() (or calibrate() in NEST 3.3 and older).
  **/
  hh_psc_alpha2_nestml(const hh_psc_alpha2_nestml &);

  /**
   * Destructor.
  **/
  ~hh_psc_alpha2_nestml() override;

  // -------------------------------------------------------------------------
  //   Import sets of overloaded virtual functions.
  //   See: Technical Issues / Virtual Functions: Overriding, Overloading,
  //        and Hiding
  // -------------------------------------------------------------------------

  using nest::Node::handles_test_event;
  using nest::Node::handle;

  /**
   * Used to validate that we can send nest::SpikeEvent to desired target:port.
  **/
  nest_port_t send_test_event(nest::Node& target, nest_rport_t receptor_type, nest::synindex, bool) override;


  // -------------------------------------------------------------------------
  //   Functions handling incoming events.
  //   We tell nest that we can handle incoming events of various types by
  //   defining handle() for the given event.
  // -------------------------------------------------------------------------


  void handle(nest::SpikeEvent &) override;        //! accept spikes
  void handle(nest::CurrentEvent &) override;      //! accept input current

  void handle(nest::DataLoggingRequest &) override;//! allow recording with multimeter
  nest_port_t handles_test_event(nest::SpikeEvent&, nest_port_t) override;
  nest_port_t handles_test_event(nest::CurrentEvent&, nest_port_t) override;
  nest_port_t handles_test_event(nest::DataLoggingRequest&, nest_port_t) override;

  // -------------------------------------------------------------------------
  //   Functions for getting/setting parameters and state values.
  // -------------------------------------------------------------------------

  void get_status(DictionaryDatum &) const override;
  void set_status(const DictionaryDatum &) override;


  // -------------------------------------------------------------------------
  //   Getters/setters for state block
  // -------------------------------------------------------------------------

  inline long get_r() const
  {
    return S_.r;
  }

  inline void set_r(const long __v)
  {
    S_.r = __v;
  }

  inline double get_V_m() const
  {
    return S_.ode_state[State_::V_m];
  }

  inline void set_V_m(const double __v)
  {
    S_.ode_state[State_::V_m] = __v;
  }

  inline double get_Act_m() const
  {
    return S_.ode_state[State_::Act_m];
  }

  inline void set_Act_m(const double __v)
  {
    S_.ode_state[State_::Act_m] = __v;
  }

  inline double get_Inact_h() const
  {
    return S_.ode_state[State_::Inact_h];
  }

  inline void set_Inact_h(const double __v)
  {
    S_.ode_state[State_::Inact_h] = __v;
  }

  inline double get_Act_n() const
  {
    return S_.ode_state[State_::Act_n];
  }

  inline void set_Act_n(const double __v)
  {
    S_.ode_state[State_::Act_n] = __v;
  }

  inline double get_Actm_NaP() const
  {
    return S_.ode_state[State_::Actm_NaP];
  }

  inline void set_Actm_NaP(const double __v)
  {
    S_.ode_state[State_::Actm_NaP] = __v;
  }

  inline double get_Inacth_NaP() const
  {
    return S_.ode_state[State_::Inacth_NaP];
  }

  inline void set_Inacth_NaP(const double __v)
  {
    S_.ode_state[State_::Inacth_NaP] = __v;
  }

  inline double get_K_syn_inh__X__inh_spikes() const
  {
    return S_.ode_state[State_::K_syn_inh__X__inh_spikes];
  }

  inline void set_K_syn_inh__X__inh_spikes(const double __v)
  {
    S_.ode_state[State_::K_syn_inh__X__inh_spikes] = __v;
  }

  inline double get_K_syn_inh__X__inh_spikes__d() const
  {
    return S_.ode_state[State_::K_syn_inh__X__inh_spikes__d];
  }

  inline void set_K_syn_inh__X__inh_spikes__d(const double __v)
  {
    S_.ode_state[State_::K_syn_inh__X__inh_spikes__d] = __v;
  }

  inline double get_K_syn_exc__X__exc_spikes() const
  {
    return S_.ode_state[State_::K_syn_exc__X__exc_spikes];
  }

  inline void set_K_syn_exc__X__exc_spikes(const double __v)
  {
    S_.ode_state[State_::K_syn_exc__X__exc_spikes] = __v;
  }

  inline double get_K_syn_exc__X__exc_spikes__d() const
  {
    return S_.ode_state[State_::K_syn_exc__X__exc_spikes__d];
  }

  inline void set_K_syn_exc__X__exc_spikes__d(const double __v)
  {
    S_.ode_state[State_::K_syn_exc__X__exc_spikes__d] = __v;
  }


  // -------------------------------------------------------------------------
  //   Getters/setters for parameters
  // -------------------------------------------------------------------------

  inline double get_t_ref() const
  {
    return P_.t_ref;
  }

  inline void set_t_ref(const double __v)
  {
    P_.t_ref = __v;
  }

  inline double get_g_Na() const
  {
    return P_.g_Na;
  }

  inline void set_g_Na(const double __v)
  {
    P_.g_Na = __v;
  }

  inline double get_g_K() const
  {
    return P_.g_K;
  }

  inline void set_g_K(const double __v)
  {
    P_.g_K = __v;
  }

  inline double get_g_L() const
  {
    return P_.g_L;
  }

  inline void set_g_L(const double __v)
  {
    P_.g_L = __v;
  }

  inline double get_g_NaP() const
  {
    return P_.g_NaP;
  }

  inline void set_g_NaP(const double __v)
  {
    P_.g_NaP = __v;
  }

  inline double get_C_m() const
  {
    return P_.C_m;
  }

  inline void set_C_m(const double __v)
  {
    P_.C_m = __v;
  }

  inline double get_E_Na() const
  {
    return P_.E_Na;
  }

  inline void set_E_Na(const double __v)
  {
    P_.E_Na = __v;
  }

  inline double get_E_K() const
  {
    return P_.E_K;
  }

  inline void set_E_K(const double __v)
  {
    P_.E_K = __v;
  }

  inline double get_E_L() const
  {
    return P_.E_L;
  }

  inline void set_E_L(const double __v)
  {
    P_.E_L = __v;
  }

  inline double get_tau_syn_exc() const
  {
    return P_.tau_syn_exc;
  }

  inline void set_tau_syn_exc(const double __v)
  {
    P_.tau_syn_exc = __v;
  }

  inline double get_E_exc() const
  {
    return P_.E_exc;
  }

  inline void set_E_exc(const double __v)
  {
    P_.E_exc = __v;
  }

  inline double get_tau_syn_inh() const
  {
    return P_.tau_syn_inh;
  }

  inline void set_tau_syn_inh(const double __v)
  {
    P_.tau_syn_inh = __v;
  }

  inline double get_E_inh() const
  {
    return P_.E_inh;
  }

  inline void set_E_inh(const double __v)
  {
    P_.E_inh = __v;
  }

  inline double get_V_m_init() const
  {
    return P_.V_m_init;
  }

  inline void set_V_m_init(const double __v)
  {
    P_.V_m_init = __v;
  }

  inline double get_alpha_n_init() const
  {
    return P_.alpha_n_init;
  }

  inline void set_alpha_n_init(const double __v)
  {
    P_.alpha_n_init = __v;
  }

  inline double get_beta_n_init() const
  {
    return P_.beta_n_init;
  }

  inline void set_beta_n_init(const double __v)
  {
    P_.beta_n_init = __v;
  }

  inline double get_alpha_m_init() const
  {
    return P_.alpha_m_init;
  }

  inline void set_alpha_m_init(const double __v)
  {
    P_.alpha_m_init = __v;
  }

  inline double get_beta_m_init() const
  {
    return P_.beta_m_init;
  }

  inline void set_beta_m_init(const double __v)
  {
    P_.beta_m_init = __v;
  }

  inline double get_alpha_h_init() const
  {
    return P_.alpha_h_init;
  }

  inline void set_alpha_h_init(const double __v)
  {
    P_.alpha_h_init = __v;
  }

  inline double get_beta_h_init() const
  {
    return P_.beta_h_init;
  }

  inline void set_beta_h_init(const double __v)
  {
    P_.beta_h_init = __v;
  }

  inline double get_I_e() const
  {
    return P_.I_e;
  }

  inline void set_I_e(const double __v)
  {
    P_.I_e = __v;
  }


  // -------------------------------------------------------------------------
  //   Getters/setters for internals
  // -------------------------------------------------------------------------

  inline long get_RefractoryCounts() const
  {
    return V_.RefractoryCounts;
  }

  inline void set_RefractoryCounts(const long __v)
  {
    V_.RefractoryCounts = __v;
  }
  inline double get___h() const
  {
    return V_.__h;
  }

  inline void set___h(const double __v)
  {
    V_.__h = __v;
  }
  inline double get___P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes() const
  {
    return V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes;
  }

  inline void set___P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes(const double __v)
  {
    V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes = __v;
  }
  inline double get___P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d() const
  {
    return V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d;
  }

  inline void set___P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d(const double __v)
  {
    V_.__P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d = __v;
  }
  inline double get___P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes() const
  {
    return V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes;
  }

  inline void set___P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes(const double __v)
  {
    V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes = __v;
  }
  inline double get___P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d() const
  {
    return V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d;
  }

  inline void set___P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d(const double __v)
  {
    V_.__P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d = __v;
  }
  inline double get___P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes() const
  {
    return V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes;
  }

  inline void set___P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes(const double __v)
  {
    V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes = __v;
  }
  inline double get___P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d() const
  {
    return V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d;
  }

  inline void set___P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d(const double __v)
  {
    V_.__P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d = __v;
  }
  inline double get___P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes() const
  {
    return V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes;
  }

  inline void set___P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes(const double __v)
  {
    V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes = __v;
  }
  inline double get___P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d() const
  {
    return V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d;
  }

  inline void set___P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d(const double __v)
  {
    V_.__P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d = __v;
  }


  // -------------------------------------------------------------------------
  //   Initialization functions
  // -------------------------------------------------------------------------
  void calibrate_time( const nest::TimeConverter& tc ) override;

protected:

private:
  void recompute_internal_variables(bool exclude_timestep=false);

private:

  static const nest_port_t MIN_SPIKE_RECEPTOR = 0;
  static const nest_port_t PORT_NOT_AVAILABLE = -1;

  enum SynapseTypes
  {
    INH_SPIKES = 0,
    EXC_SPIKES = 1,
    MAX_SPIKE_RECEPTOR = 2
  };

  static const size_t NUM_SPIKE_RECEPTORS = MAX_SPIKE_RECEPTOR - MIN_SPIKE_RECEPTOR;

static std::vector< std::tuple< int, int > > rport_to_nestml_buffer_idx;

  /**
   * Reset state of neuron.
  **/

  void init_state_internal_();

  /**
   * Reset internal buffers of neuron.
  **/
  void init_buffers_() override;

  /**
   * Initialize auxiliary quantities, leave parameters and state untouched.
  **/
  void pre_run_hook() override;

  /**
   * Take neuron through given time interval
  **/
  void update(nest::Time const &, const long, const long) override;

  // The next two classes need to be friends to access the State_ class/member
  friend class nest::RecordablesMap<hh_psc_alpha2_nestml>;
  friend class nest::UniversalDataLogger<hh_psc_alpha2_nestml>;

  /**
   * Free parameters of the neuron.
   *


   *
   * These are the parameters that can be set by the user through @c `node.set()`.
   * They are initialized from the model prototype when the node is created.
   * Parameters do not change during calls to @c update() and are not reset by
   * @c ResetNetwork.
   *
   * @note Parameters_ need neither copy constructor nor @c operator=(), since
   *       all its members are copied properly by the default copy constructor
   *       and assignment operator. Important:
   *       - If Parameters_ contained @c Time members, you need to define the
   *         assignment operator to recalibrate all members of type @c Time . You
   *         may also want to define the assignment operator.
   *       - If Parameters_ contained members that cannot copy themselves, such
   *         as C-style arrays, you need to define the copy constructor and
   *         assignment operator to copy those members.
  **/
  struct Parameters_
  {    
    //!  Refractory period
    double t_ref;
    //!  Sodium peak conductance
    double g_Na;
    //!  Potassium peak conductance
    double g_K;
    //!  Leak conductance
    double g_L;
    //!  persistent Sodium Conductance
    double g_NaP;
    //!  Membrane Capacitance
    double C_m;
    //!  Sodium reversal potential
    double E_Na;
    //!  Potassium reversal potential
    double E_K;
    //!  Leak reversal Potential (aka resting potential)
    double E_L;
    //!  Rise time of the excitatory synaptic alpha function
    double tau_syn_exc;
    //!  Excitatory reversal Potential
    double E_exc;
    //!  Rise time of the inhibitory synaptic alpha function
    double tau_syn_inh;
    //!  Inhibitory reversal Potential
    double E_inh;
    //!  Initial membrane potential
    double V_m_init;
    double alpha_n_init;
    double beta_n_init;
    double alpha_m_init;
    double beta_m_init;
    double alpha_h_init;
    double beta_h_init;
    //!  constant external input current
    double I_e;

    double __gsl_error_tol;

    /**
     * Initialize parameters to their default values.
    **/
    Parameters_();
  };

  /**
   * Dynamic state of the neuron.
   *
   *
   *
   * These are the state variables that are advanced in time by calls to
   * @c update(). In many models, some or all of them can be set by the user
   * through @c `node.set()`. The state variables are initialized from the model
   * prototype when the node is created. State variables are reset by @c ResetNetwork.
   *
   * @note State_ need neither copy constructor nor @c operator=(), since
   *       all its members are copied properly by the default copy constructor
   *       and assignment operator. Important:
   *       - If State_ contained @c Time members, you need to define the
   *         assignment operator to recalibrate all members of type @c Time . You
   *         may also want to define the assignment operator.
   *       - If State_ contained members that cannot copy themselves, such
   *         as C-style arrays, you need to define the copy constructor and
   *         assignment operator to copy those members.
  **/
  struct State_
  {

    // non-ODE state variables
//!  number of steps in the current refractory phase
long r;
    //! Symbolic indices to the elements of the state vector y
    enum StateVecElems
    {
      Act_n,
      Act_m,
      Inact_h,
      Actm_NaP,
      Inacth_NaP,
      V_m,
      K_syn_inh__X__inh_spikes,
      K_syn_inh__X__inh_spikes__d,
      K_syn_exc__X__exc_spikes,
      K_syn_exc__X__exc_spikes__d,
      // moved state variables from synapse (numeric)
      // moved state variables from synapse (analytic)
      // final entry to easily get the vector size
      STATE_VEC_SIZE
    };

    //! state vector, must be C-array for GSL solver
    double ode_state[STATE_VEC_SIZE];

    State_();
  };

  struct DelayedVariables_
  {
  };

  /**
   * Internal variables of the neuron.
   *
   *
   *
   * These variables must be initialized by @c pre_run_hook (or calibrate in NEST 3.3 and older), which is called before
   * the first call to @c update() upon each call to @c Simulate.
   * @node Variables_ needs neither constructor, copy constructor or assignment operator,
   *       since it is initialized by @c pre_run_hook() (or calibrate() in NEST 3.3 and older). If Variables_ has members that
   *       cannot destroy themselves, Variables_ will need a destructor.
  **/
  struct Variables_
  {
    //!  refractory time in steps
    long RefractoryCounts;
    double __h;
    double __P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes;
    double __P__K_syn_inh__X__inh_spikes__K_syn_inh__X__inh_spikes__d;
    double __P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes;
    double __P__K_syn_inh__X__inh_spikes__d__K_syn_inh__X__inh_spikes__d;
    double __P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes;
    double __P__K_syn_exc__X__exc_spikes__K_syn_exc__X__exc_spikes__d;
    double __P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes;
    double __P__K_syn_exc__X__exc_spikes__d__K_syn_exc__X__exc_spikes__d;
  };

  /**
   * Buffers of the neuron.
   * Usually buffers for incoming spikes and data logged for analog recorders.
   * Buffers must be initialized by @c init_buffers_(), which is called before
   * @c pre_run_hook() (or calibrate() in NEST 3.3 and older) on the first call to @c Simulate after the start of NEST,
   * ResetKernel or ResetNetwork.
   * @node Buffers_ needs neither constructor, copy constructor or assignment operator,
   *       since it is initialized by @c init_nodes_(). If Buffers_ has members that
   *       cannot destroy themselves, Buffers_ will need a destructor.
  **/
  struct Buffers_
  {
    Buffers_(hh_psc_alpha2_nestml &);
    Buffers_(const Buffers_ &, hh_psc_alpha2_nestml &);

    /**
     * Logger for all analog data
    **/
    nest::UniversalDataLogger<hh_psc_alpha2_nestml> logger_;

    // -----------------------------------------------------------------------
    //   Buffers and sums of incoming spikes/currents per timestep
    // -----------------------------------------------------------------------
    // Buffer containing the incoming spikes
    

inline std::vector< nest::RingBuffer >& get_spike_inputs_()
{
    return spike_inputs_;
}
std::vector< nest::RingBuffer > spike_inputs_;

    // Buffer containing the sum of all the incoming spikes
    

inline std::vector< double >& get_spike_inputs_grid_sum_()
{
    return spike_inputs_grid_sum_;
}
std::vector< double > spike_inputs_grid_sum_;

nest::RingBuffer
 I_stim;   //!< Buffer for input (type: pA)    
    inline nest::RingBuffer& get_I_stim() {
        return I_stim;
    }

double I_stim_grid_sum_;

    // -----------------------------------------------------------------------
    //   GSL ODE solver data structures
    // -----------------------------------------------------------------------

    gsl_odeiv_step* __s;    //!< stepping function
    gsl_odeiv_control* __c; //!< adaptive stepsize control function
    gsl_odeiv_evolve* __e;  //!< evolution function
    gsl_odeiv_system __sys; //!< struct describing system

    // __integration_step should be reset with the neuron on ResetNetwork,
    // but remain unchanged during calibration. Since it is initialized with
    // step_, and the resolution cannot change after nodes have been created,
    // it is safe to place both here.
    double __step;             //!< step size in ms
    double __integration_step; //!< current integration time step, updated by GSL
  };

  // -------------------------------------------------------------------------
  //   Getters/setters for inline expressions
  // -------------------------------------------------------------------------
  inline double get_I_syn_exc() const
  {
    return S_.ode_state[State_::K_syn_exc__X__exc_spikes] * 1.0 * (S_.ode_state[State_::V_m] - P_.E_exc);
  }

  inline double get_I_syn_inh() const
  {
    return S_.ode_state[State_::K_syn_inh__X__inh_spikes] * 1.0 * (S_.ode_state[State_::V_m] - P_.E_inh);
  }

  inline double get_I_Na() const
  {
    return P_.g_Na * S_.ode_state[State_::Act_m] * S_.ode_state[State_::Act_m] * S_.ode_state[State_::Act_m] * S_.ode_state[State_::Inact_h] * (S_.ode_state[State_::V_m] - P_.E_Na);
  }

  inline double get_I_K() const
  {
    return P_.g_K * S_.ode_state[State_::Act_n] * S_.ode_state[State_::Act_n] * S_.ode_state[State_::Act_n] * S_.ode_state[State_::Act_n] * (S_.ode_state[State_::V_m] - P_.E_K);
  }

  inline double get_I_L() const
  {
    return P_.g_L * (S_.ode_state[State_::V_m] - P_.E_L);
  }

  inline double get_I_NaP() const
  {
    return P_.g_NaP * S_.ode_state[State_::Actm_NaP] * S_.ode_state[State_::Inacth_NaP] * (S_.ode_state[State_::V_m] - P_.E_Na);
  }

  inline double get_alpha_n() const
  {
    return (0.01 * (S_.ode_state[State_::V_m] / 1.0 + 55.0)) / (1.0 - std::exp((-(S_.ode_state[State_::V_m] / 1.0 + 55.0)) / 10.0));
  }

  inline double get_beta_n() const
  {
    return 0.125 * std::exp((-(S_.ode_state[State_::V_m] / 1.0 + 65.0)) / 80.0);
  }

  inline double get_alpha_m() const
  {
    return (0.1 * (S_.ode_state[State_::V_m] / 1.0 + 40.0)) / (1.0 - std::exp((-(S_.ode_state[State_::V_m] / 1.0 + 40.0)) / 10.0));
  }

  inline double get_beta_m() const
  {
    return 4.0 * std::exp((-(S_.ode_state[State_::V_m] / 1.0 + 65.0)) / 18.0);
  }

  inline double get_alpha_h() const
  {
    return 0.07 * std::exp((-(S_.ode_state[State_::V_m] / 1.0 + 65.0)) / 20.0);
  }

  inline double get_beta_h() const
  {
    return 1.0 / (1.0 + std::exp((-(S_.ode_state[State_::V_m] / 1.0 + 35.0)) / 10.0));
  }



  // -------------------------------------------------------------------------
  //   Getters/setters for input buffers
  // -------------------------------------------------------------------------

  // Buffer containing the incoming spikes
  

inline std::vector< nest::RingBuffer >& get_spike_inputs_()
{
    return B_.get_spike_inputs_();
}

  

inline std::vector< double >& get_spike_inputs_grid_sum_()
{
    return B_.get_spike_inputs_grid_sum_();
}
  
inline nest::RingBuffer& get_I_stim() {
    return B_.get_I_stim();
}
  // -------------------------------------------------------------------------
  //   Function declarations
  // -------------------------------------------------------------------------



  //
  double m_inf ( double V_m) const;

  //
  double Tau_m ( double V_m) const;

  //
  double h_inf ( double V_m) const;

  //
  double Tau_h ( double V_m) const;

  // -------------------------------------------------------------------------
  //   Member variables of neuron model.
  //   Each model neuron should have precisely the following four data members,
  //   which are one instance each of the parameters, state, buffers and variables
  //   structures. Experience indicates that the state and variables member should
  //   be next to each other to achieve good efficiency (caching).
  //   Note: Devices require one additional data member, an instance of the
  //   ``Device`` child class they belong to.
  // -------------------------------------------------------------------------


  Parameters_       P_;        //!< Free parameters.
  State_            S_;        //!< Dynamic state.
  DelayedVariables_ DV_;       //!< Delayed state variables.
  Variables_        V_;        //!< Internal Variables
  Buffers_          B_;        //!< Buffers.

  //! Mapping of recordables names to access functions
  static nest::RecordablesMap<hh_psc_alpha2_nestml> recordablesMap_;
  friend int hh_psc_alpha2_nestml_dynamics( double, const double y[], double f[], void* pnode );


}; /* neuron hh_psc_alpha2_nestml */

inline nest_port_t hh_psc_alpha2_nestml::send_test_event(nest::Node& target, nest_rport_t receptor_type, nest::synindex, bool)
{
  // You should usually not change the code in this function.
  // It confirms that the target of connection @c c accepts @c nest::SpikeEvent on
  // the given @c receptor_type.
  nest::SpikeEvent e;
  e.set_sender(*this);
  return target.handles_test_event(e, receptor_type);
}

inline nest_port_t hh_psc_alpha2_nestml::handles_test_event(nest::SpikeEvent&, nest_port_t receptor_type)
{
    // You should usually not change the code in this function.
    // It confirms to the connection management system that we are able
    // to handle @c SpikeEvent on port 0. You need to extend the function
    // if you want to differentiate between input ports.
    if (receptor_type != 0)
    {
      throw nest::UnknownReceptorType(receptor_type, get_name());
    }
    return 0;
}

inline nest_port_t hh_psc_alpha2_nestml::handles_test_event(nest::CurrentEvent&, nest_port_t receptor_type)
{
  // You should usually not change the code in this function.
  // It confirms to the connection management system that we are able
  // to handle @c CurrentEvent on port 0. You need to extend the function
  // if you want to differentiate between input ports.
  if (receptor_type != 0)
  {
    throw nest::UnknownReceptorType(receptor_type, get_name());
  }
  return 0;
}

inline nest_port_t hh_psc_alpha2_nestml::handles_test_event(nest::DataLoggingRequest& dlr, nest_port_t receptor_type)
{
  // You should usually not change the code in this function.
  // It confirms to the connection management system that we are able
  // to handle @c DataLoggingRequest on port 0.
  // The function also tells the built-in UniversalDataLogger that this node
  // is recorded from and that it thus needs to collect data during simulation.
  if (receptor_type != 0)
  {
    throw nest::UnknownReceptorType(receptor_type, get_name());
  }

  return B_.logger_.connect_logging_device(dlr, recordablesMap_);
}

inline void hh_psc_alpha2_nestml::get_status(DictionaryDatum &__d) const
{
  // parameters
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_t_ref, get_t_ref());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_Na, get_g_Na());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_K, get_g_K());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_L, get_g_L());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_NaP, get_g_NaP());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_C_m, get_C_m());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_Na, get_E_Na());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_K, get_E_K());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_L, get_E_L());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_tau_syn_exc, get_tau_syn_exc());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_exc, get_E_exc());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_tau_syn_inh, get_tau_syn_inh());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_inh, get_E_inh());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_V_m_init, get_V_m_init());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_alpha_n_init, get_alpha_n_init());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_beta_n_init, get_beta_n_init());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_alpha_m_init, get_alpha_m_init());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_beta_m_init, get_beta_m_init());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_alpha_h_init, get_alpha_h_init());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_beta_h_init, get_beta_h_init());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_I_e, get_I_e());

  // initial values for state variables in ODE or kernel
  def<long>(__d, nest::hh_psc_alpha2_nestml_names::_r, get_r());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_V_m, get_V_m());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_Act_m, get_Act_m());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_Inact_h, get_Inact_h());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_Act_n, get_Act_n());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_Actm_NaP, get_Actm_NaP());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_Inacth_NaP, get_Inacth_NaP());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_inh__X__inh_spikes, get_K_syn_inh__X__inh_spikes());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_inh__X__inh_spikes__d, get_K_syn_inh__X__inh_spikes__d());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_exc__X__exc_spikes, get_K_syn_exc__X__exc_spikes());
  def<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_exc__X__exc_spikes__d, get_K_syn_exc__X__exc_spikes__d());

  ArchivingNode::get_status( __d );

  (*__d)[nest::names::recordables] = recordablesMap_.get_list();
  def< double >(__d, nest::names::gsl_error_tol, P_.__gsl_error_tol);
  if ( P_.__gsl_error_tol <= 0. ){
    throw nest::BadProperty( "The gsl_error_tol must be strictly positive." );
  }
}

inline void hh_psc_alpha2_nestml::set_status(const DictionaryDatum &__d)
{
  // parameters
  double tmp_t_ref = get_t_ref();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_t_ref, tmp_t_ref, this);
  double tmp_g_Na = get_g_Na();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_Na, tmp_g_Na, this);
  double tmp_g_K = get_g_K();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_K, tmp_g_K, this);
  double tmp_g_L = get_g_L();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_L, tmp_g_L, this);
  double tmp_g_NaP = get_g_NaP();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_g_NaP, tmp_g_NaP, this);
  double tmp_C_m = get_C_m();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_C_m, tmp_C_m, this);
  double tmp_E_Na = get_E_Na();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_Na, tmp_E_Na, this);
  double tmp_E_K = get_E_K();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_K, tmp_E_K, this);
  double tmp_E_L = get_E_L();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_L, tmp_E_L, this);
  double tmp_tau_syn_exc = get_tau_syn_exc();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_tau_syn_exc, tmp_tau_syn_exc, this);
  double tmp_E_exc = get_E_exc();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_exc, tmp_E_exc, this);
  double tmp_tau_syn_inh = get_tau_syn_inh();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_tau_syn_inh, tmp_tau_syn_inh, this);
  double tmp_E_inh = get_E_inh();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_E_inh, tmp_E_inh, this);
  double tmp_V_m_init = get_V_m_init();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_V_m_init, tmp_V_m_init, this);
  double tmp_alpha_n_init = get_alpha_n_init();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_alpha_n_init, tmp_alpha_n_init, this);
  double tmp_beta_n_init = get_beta_n_init();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_beta_n_init, tmp_beta_n_init, this);
  double tmp_alpha_m_init = get_alpha_m_init();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_alpha_m_init, tmp_alpha_m_init, this);
  double tmp_beta_m_init = get_beta_m_init();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_beta_m_init, tmp_beta_m_init, this);
  double tmp_alpha_h_init = get_alpha_h_init();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_alpha_h_init, tmp_alpha_h_init, this);
  double tmp_beta_h_init = get_beta_h_init();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_beta_h_init, tmp_beta_h_init, this);
  double tmp_I_e = get_I_e();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_I_e, tmp_I_e, this);

  // initial values for state variables in ODE or kernel
  long tmp_r = get_r();
  nest::updateValueParam<long>(__d, nest::hh_psc_alpha2_nestml_names::_r, tmp_r, this);
  double tmp_V_m = get_V_m();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_V_m, tmp_V_m, this);
  double tmp_Act_m = get_Act_m();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_Act_m, tmp_Act_m, this);
  double tmp_Inact_h = get_Inact_h();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_Inact_h, tmp_Inact_h, this);
  double tmp_Act_n = get_Act_n();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_Act_n, tmp_Act_n, this);
  double tmp_Actm_NaP = get_Actm_NaP();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_Actm_NaP, tmp_Actm_NaP, this);
  double tmp_Inacth_NaP = get_Inacth_NaP();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_Inacth_NaP, tmp_Inacth_NaP, this);
  double tmp_K_syn_inh__X__inh_spikes = get_K_syn_inh__X__inh_spikes();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_inh__X__inh_spikes, tmp_K_syn_inh__X__inh_spikes, this);
  double tmp_K_syn_inh__X__inh_spikes__d = get_K_syn_inh__X__inh_spikes__d();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_inh__X__inh_spikes__d, tmp_K_syn_inh__X__inh_spikes__d, this);
  double tmp_K_syn_exc__X__exc_spikes = get_K_syn_exc__X__exc_spikes();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_exc__X__exc_spikes, tmp_K_syn_exc__X__exc_spikes, this);
  double tmp_K_syn_exc__X__exc_spikes__d = get_K_syn_exc__X__exc_spikes__d();
  nest::updateValueParam<double>(__d, nest::hh_psc_alpha2_nestml_names::_K_syn_exc__X__exc_spikes__d, tmp_K_syn_exc__X__exc_spikes__d, this);

  // We now know that (ptmp, stmp) are consistent. We do not
  // write them back to (P_, S_) before we are also sure that
  // the properties to be set in the parent class are internally
  // consistent.
  ArchivingNode::set_status(__d);

  // if we get here, temporaries contain consistent set of properties
  set_t_ref(tmp_t_ref);
  set_g_Na(tmp_g_Na);
  set_g_K(tmp_g_K);
  set_g_L(tmp_g_L);
  set_g_NaP(tmp_g_NaP);
  set_C_m(tmp_C_m);
  set_E_Na(tmp_E_Na);
  set_E_K(tmp_E_K);
  set_E_L(tmp_E_L);
  set_tau_syn_exc(tmp_tau_syn_exc);
  set_E_exc(tmp_E_exc);
  set_tau_syn_inh(tmp_tau_syn_inh);
  set_E_inh(tmp_E_inh);
  set_V_m_init(tmp_V_m_init);
  set_alpha_n_init(tmp_alpha_n_init);
  set_beta_n_init(tmp_beta_n_init);
  set_alpha_m_init(tmp_alpha_m_init);
  set_beta_m_init(tmp_beta_m_init);
  set_alpha_h_init(tmp_alpha_h_init);
  set_beta_h_init(tmp_beta_h_init);
  set_I_e(tmp_I_e);
  set_r(tmp_r);
  set_V_m(tmp_V_m);
  set_Act_m(tmp_Act_m);
  set_Inact_h(tmp_Inact_h);
  set_Act_n(tmp_Act_n);
  set_Actm_NaP(tmp_Actm_NaP);
  set_Inacth_NaP(tmp_Inacth_NaP);
  set_K_syn_inh__X__inh_spikes(tmp_K_syn_inh__X__inh_spikes);
  set_K_syn_inh__X__inh_spikes__d(tmp_K_syn_inh__X__inh_spikes__d);
  set_K_syn_exc__X__exc_spikes(tmp_K_syn_exc__X__exc_spikes);
  set_K_syn_exc__X__exc_spikes__d(tmp_K_syn_exc__X__exc_spikes__d);




  updateValue< double >(__d, nest::names::gsl_error_tol, P_.__gsl_error_tol);
  if ( P_.__gsl_error_tol <= 0. )
  {
    throw nest::BadProperty( "The gsl_error_tol must be strictly positive." );
  }

  // recompute internal variables in case they are dependent on parameters or state that might have been updated in this call to set_status()
  recompute_internal_variables();
};



#endif /* #ifndef HH_PSC_ALPHA2_NESTML */
