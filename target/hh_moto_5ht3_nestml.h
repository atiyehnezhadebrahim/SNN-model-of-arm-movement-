
/**
 *  hh_moto_5ht3_nestml.h
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
 *  Generated from NESTML at time: 2024-12-23 19:07:42.226079
**/
#ifndef HH_MOTO_5HT3_NESTML
#define HH_MOTO_5HT3_NESTML

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
namespace hh_moto_5ht3_nestml_names
{
    const Name _r( "r" );
    const Name _V_m( "V_m" );
    const Name _Ca_in( "Ca_in" );
    const Name _Act_m( "Act_m" );
    const Name _Act_h( "Act_h" );
    const Name _Inact_n( "Inact_n" );
    const Name _Act_p( "Act_p" );
    const Name _Act_mc( "Act_mc" );
    const Name _Act_hc( "Act_hc" );
    const Name _Actm_NaP( "Actm_NaP" );
    const Name _Inacth_NaP( "Inacth_NaP" );
    const Name _I_syn_in__X__inh_spikes( "I_syn_in__X__inh_spikes" );
    const Name _I_syn_in__X__inh_spikes__d( "I_syn_in__X__inh_spikes__d" );
    const Name _I_syn_ex__X__exc_spikes( "I_syn_ex__X__exc_spikes" );
    const Name _I_syn_ex__X__exc_spikes__d( "I_syn_ex__X__exc_spikes__d" );
    const Name _I_syn_exc( "I_syn_exc" );
    const Name _I_syn_inh( "I_syn_inh" );
    const Name _I_K_Ca( "I_K_Ca" );
    const Name _t_ref( "t_ref" );
    const Name _g_Na( "g_Na" );
    const Name _g_L( "g_L" );
    const Name _g_K_rect( "g_K_rect" );
    const Name _g_Ca_N( "g_Ca_N" );
    const Name _g_Ca_L( "g_Ca_L" );
    const Name _g_K_Ca( "g_K_Ca" );
    const Name _g_K_Ca_5ht( "g_K_Ca_5ht" );
    const Name _g_NaP( "g_NaP" );
    const Name _Ca_in_init( "Ca_in_init" );
    const Name _Ca_out( "Ca_out" );
    const Name _C_m( "C_m" );
    const Name _E_Na( "E_Na" );
    const Name _E_K( "E_K" );
    const Name _E_L( "E_L" );
    const Name _R_const( "R_const" );
    const Name _F_const( "F_const" );
    const Name _T_current( "T_current" );
    const Name _tau_syn_ex( "tau_syn_ex" );
    const Name _tau_syn_in( "tau_syn_in" );
    const Name _I_e( "I_e" );
    const Name _V_m_init( "V_m_init" );
    const Name _hc_tau( "hc_tau" );
    const Name _mc_tau( "mc_tau" );
    const Name _p_tau( "p_tau" );
    const Name _alpha( "alpha" );
    const Name _E_exc( "E_exc" );
    const Name _E_inh( "E_inh" );
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
extern "C" inline int hh_moto_5ht3_nestml_dynamics( double, const double y[], double f[], void* pnode );


#include "nest_time.h"
  typedef nest::port nest_port_t;
  typedef nest::rport nest_rport_t;

/* BeginDocumentation
  Name: hh_moto_5ht3_nestml

  Description:

    """
  hh_moto_5ht_nestml - a motor neuron model in HH formalism with 5HT modulation############################################################################

  Description
  +++++++++++

  hh_moto_5ht is an implementation of a spiking motor neuron using the Hodgkin-Huxley formalism according to [2]_. Basically this model is an implementation of the existing NEURON model [1]_.

  The parameter that represents 5HT modulation is ``g_K_Ca_5ht``. When it equals 1, no modulation happens. An application of 5HT corresponds to its decrease. The default value for it is 0.6. This value was used in the Neuron simulator model. The range of this parameter is (0, 1] but you are free to play with any value.

  Post-synaptic currents and spike detection are the same as in hh_psc_alpha.


  References
  ++++++++++

  .. [1] Muscle spindle feedback circuit by Moraud EM and Capogrosso M.
         https://senselab.med.yale.edu/ModelDB/showmodel.cshtml?model=189786

  .. [2] Compartmental model of vertebrate motoneurons for Ca2+-dependent spiking and plateau potentials under pharmacological treatment.
         Booth V, Rinzel J, Kiehn O.
         http://refhub.elsevier.com/S0896-6273(16)00010-6/sref4

  .. [3] Repository: https://github.com/research-team/hh-moto-5ht


  See also
  ++++++++

  hh_psc_alpha
  """


  Parameters:
  The following parameters can be set in the status dictionary.
t_ref [ms]  Refractory period
g_Na [nS]  Sodium peak conductance
g_L [nS]  Leak conductance
g_K_rect [nS]  Delayed Rectifier Potassium peak conductance
g_K_Ca_5ht [real]  modulation of K-Ca channels by 5HT. Its value 1.0 == no modulation.
g_NaP [nS]  persistent Sodium Conductance
Ca_in_init [mmol]  Initial inside Calcium concentration
Ca_out [mmol]  Outside Calcium concentration. Remains constant during simulation.
C_m [pF]  Membrane capacitance
R_const [real]  Nernst equation constants
T_current [real]  36 Celcius
tau_syn_ex [ms]  Rise time of the excitatory synaptic alpha function
tau_syn_in [ms]  Rise time of the inhibitory synaptic alpha function
I_e [pA]  Constant current
E_exc [mV]  Excitatory reversal Potential
E_inh [mV]  Inhibitory reversal Potential


  Dynamic state variables:
r [integer]  number of steps in the current refractory phase
V_m [mV]  Membrane potential
Ca_in [mmol]  Inside Calcium concentration


  Sends: nest::SpikeEvent

  Receives: Spike, Current, DataLoggingRequest
*/

// Register the neuron model

class hh_moto_5ht3_nestml : public nest::ArchivingNode
{
public:
  /**
   * The constructor is only used to create the model prototype in the model manager.
  **/
  hh_moto_5ht3_nestml();

  /**
   * The copy constructor is used to create model copies and instances of the model.
   * @node The copy constructor needs to initialize the parameters and the state.
   *       Initialization of buffers and interal variables is deferred to
   *       @c init_buffers_() and @c pre_run_hook() (or calibrate() in NEST 3.3 and older).
  **/
  hh_moto_5ht3_nestml(const hh_moto_5ht3_nestml &);

  /**
   * Destructor.
  **/
  ~hh_moto_5ht3_nestml() override;

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

  inline double get_Ca_in() const
  {
    return S_.ode_state[State_::Ca_in];
  }

  inline void set_Ca_in(const double __v)
  {
    S_.ode_state[State_::Ca_in] = __v;
  }

  inline double get_Act_m() const
  {
    return S_.ode_state[State_::Act_m];
  }

  inline void set_Act_m(const double __v)
  {
    S_.ode_state[State_::Act_m] = __v;
  }

  inline double get_Act_h() const
  {
    return S_.ode_state[State_::Act_h];
  }

  inline void set_Act_h(const double __v)
  {
    S_.ode_state[State_::Act_h] = __v;
  }

  inline double get_Inact_n() const
  {
    return S_.ode_state[State_::Inact_n];
  }

  inline void set_Inact_n(const double __v)
  {
    S_.ode_state[State_::Inact_n] = __v;
  }

  inline double get_Act_p() const
  {
    return S_.ode_state[State_::Act_p];
  }

  inline void set_Act_p(const double __v)
  {
    S_.ode_state[State_::Act_p] = __v;
  }

  inline double get_Act_mc() const
  {
    return S_.ode_state[State_::Act_mc];
  }

  inline void set_Act_mc(const double __v)
  {
    S_.ode_state[State_::Act_mc] = __v;
  }

  inline double get_Act_hc() const
  {
    return S_.ode_state[State_::Act_hc];
  }

  inline void set_Act_hc(const double __v)
  {
    S_.ode_state[State_::Act_hc] = __v;
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

  inline double get_I_syn_in__X__inh_spikes() const
  {
    return S_.ode_state[State_::I_syn_in__X__inh_spikes];
  }

  inline void set_I_syn_in__X__inh_spikes(const double __v)
  {
    S_.ode_state[State_::I_syn_in__X__inh_spikes] = __v;
  }

  inline double get_I_syn_in__X__inh_spikes__d() const
  {
    return S_.ode_state[State_::I_syn_in__X__inh_spikes__d];
  }

  inline void set_I_syn_in__X__inh_spikes__d(const double __v)
  {
    S_.ode_state[State_::I_syn_in__X__inh_spikes__d] = __v;
  }

  inline double get_I_syn_ex__X__exc_spikes() const
  {
    return S_.ode_state[State_::I_syn_ex__X__exc_spikes];
  }

  inline void set_I_syn_ex__X__exc_spikes(const double __v)
  {
    S_.ode_state[State_::I_syn_ex__X__exc_spikes] = __v;
  }

  inline double get_I_syn_ex__X__exc_spikes__d() const
  {
    return S_.ode_state[State_::I_syn_ex__X__exc_spikes__d];
  }

  inline void set_I_syn_ex__X__exc_spikes__d(const double __v)
  {
    S_.ode_state[State_::I_syn_ex__X__exc_spikes__d] = __v;
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

  inline double get_g_L() const
  {
    return P_.g_L;
  }

  inline void set_g_L(const double __v)
  {
    P_.g_L = __v;
  }

  inline double get_g_K_rect() const
  {
    return P_.g_K_rect;
  }

  inline void set_g_K_rect(const double __v)
  {
    P_.g_K_rect = __v;
  }

  inline double get_g_Ca_N() const
  {
    return P_.g_Ca_N;
  }

  inline void set_g_Ca_N(const double __v)
  {
    P_.g_Ca_N = __v;
  }

  inline double get_g_Ca_L() const
  {
    return P_.g_Ca_L;
  }

  inline void set_g_Ca_L(const double __v)
  {
    P_.g_Ca_L = __v;
  }

  inline double get_g_K_Ca() const
  {
    return P_.g_K_Ca;
  }

  inline void set_g_K_Ca(const double __v)
  {
    P_.g_K_Ca = __v;
  }

  inline double get_g_K_Ca_5ht() const
  {
    return P_.g_K_Ca_5ht;
  }

  inline void set_g_K_Ca_5ht(const double __v)
  {
    P_.g_K_Ca_5ht = __v;
  }

  inline double get_g_NaP() const
  {
    return P_.g_NaP;
  }

  inline void set_g_NaP(const double __v)
  {
    P_.g_NaP = __v;
  }

  inline double get_Ca_in_init() const
  {
    return P_.Ca_in_init;
  }

  inline void set_Ca_in_init(const double __v)
  {
    P_.Ca_in_init = __v;
  }

  inline double get_Ca_out() const
  {
    return P_.Ca_out;
  }

  inline void set_Ca_out(const double __v)
  {
    P_.Ca_out = __v;
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

  inline double get_R_const() const
  {
    return P_.R_const;
  }

  inline void set_R_const(const double __v)
  {
    P_.R_const = __v;
  }

  inline double get_F_const() const
  {
    return P_.F_const;
  }

  inline void set_F_const(const double __v)
  {
    P_.F_const = __v;
  }

  inline double get_T_current() const
  {
    return P_.T_current;
  }

  inline void set_T_current(const double __v)
  {
    P_.T_current = __v;
  }

  inline double get_tau_syn_ex() const
  {
    return P_.tau_syn_ex;
  }

  inline void set_tau_syn_ex(const double __v)
  {
    P_.tau_syn_ex = __v;
  }

  inline double get_tau_syn_in() const
  {
    return P_.tau_syn_in;
  }

  inline void set_tau_syn_in(const double __v)
  {
    P_.tau_syn_in = __v;
  }

  inline double get_I_e() const
  {
    return P_.I_e;
  }

  inline void set_I_e(const double __v)
  {
    P_.I_e = __v;
  }

  inline double get_V_m_init() const
  {
    return P_.V_m_init;
  }

  inline void set_V_m_init(const double __v)
  {
    P_.V_m_init = __v;
  }

  inline double get_hc_tau() const
  {
    return P_.hc_tau;
  }

  inline void set_hc_tau(const double __v)
  {
    P_.hc_tau = __v;
  }

  inline double get_mc_tau() const
  {
    return P_.mc_tau;
  }

  inline void set_mc_tau(const double __v)
  {
    P_.mc_tau = __v;
  }

  inline double get_p_tau() const
  {
    return P_.p_tau;
  }

  inline void set_p_tau(const double __v)
  {
    P_.p_tau = __v;
  }

  inline double get_alpha() const
  {
    return P_.alpha;
  }

  inline void set_alpha(const double __v)
  {
    P_.alpha = __v;
  }

  inline double get_E_exc() const
  {
    return P_.E_exc;
  }

  inline void set_E_exc(const double __v)
  {
    P_.E_exc = __v;
  }

  inline double get_E_inh() const
  {
    return P_.E_inh;
  }

  inline void set_E_inh(const double __v)
  {
    P_.E_inh = __v;
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
  inline double get___P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes() const
  {
    return V_.__P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes;
  }

  inline void set___P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes(const double __v)
  {
    V_.__P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes = __v;
  }
  inline double get___P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes__d() const
  {
    return V_.__P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes__d;
  }

  inline void set___P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes__d(const double __v)
  {
    V_.__P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes__d = __v;
  }
  inline double get___P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes() const
  {
    return V_.__P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes;
  }

  inline void set___P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes(const double __v)
  {
    V_.__P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes = __v;
  }
  inline double get___P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes__d() const
  {
    return V_.__P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes__d;
  }

  inline void set___P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes__d(const double __v)
  {
    V_.__P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes__d = __v;
  }
  inline double get___P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes() const
  {
    return V_.__P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes;
  }

  inline void set___P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes(const double __v)
  {
    V_.__P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes = __v;
  }
  inline double get___P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes__d() const
  {
    return V_.__P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes__d;
  }

  inline void set___P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes__d(const double __v)
  {
    V_.__P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes__d = __v;
  }
  inline double get___P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes() const
  {
    return V_.__P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes;
  }

  inline void set___P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes(const double __v)
  {
    V_.__P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes = __v;
  }
  inline double get___P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes__d() const
  {
    return V_.__P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes__d;
  }

  inline void set___P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes__d(const double __v)
  {
    V_.__P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes__d = __v;
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
  friend class nest::RecordablesMap<hh_moto_5ht3_nestml>;
  friend class nest::UniversalDataLogger<hh_moto_5ht3_nestml>;

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
    //!  Leak conductance
    double g_L;
    //!  Delayed Rectifier Potassium peak conductance
    double g_K_rect;
    double g_Ca_N;
    double g_Ca_L;
    double g_K_Ca;
    //!  modulation of K-Ca channels by 5HT. Its value 1.0 == no modulation.
    double g_K_Ca_5ht;
    //!  persistent Sodium Conductance
    double g_NaP;
    //!  Initial inside Calcium concentration
    double Ca_in_init;
    //!  Outside Calcium concentration. Remains constant during simulation.
    double Ca_out;
    //!  Membrane capacitance
    double C_m;
    double E_Na;
    double E_K;
    double E_L;
    //!  Nernst equation constants
    double R_const;
    double F_const;
    //!  36 Celcius
    double T_current;
    //!  Rise time of the excitatory synaptic alpha function
    double tau_syn_ex;
    //!  Rise time of the inhibitory synaptic alpha function
    double tau_syn_in;
    //!  Constant current
    double I_e;
    double V_m_init;
    double hc_tau;
    double mc_tau;
    double p_tau;
    double alpha;
    //!  Excitatory reversal Potential
    double E_exc;
    //!  Inhibitory reversal Potential
    double E_inh;

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
      V_m,
      Inact_n,
      Act_m,
      Act_h,
      Act_p,
      Act_mc,
      Act_hc,
      Actm_NaP,
      Inacth_NaP,
      Ca_in,
      I_syn_in__X__inh_spikes,
      I_syn_in__X__inh_spikes__d,
      I_syn_ex__X__exc_spikes,
      I_syn_ex__X__exc_spikes__d,
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
    double __P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes;
    double __P__I_syn_in__X__inh_spikes__I_syn_in__X__inh_spikes__d;
    double __P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes;
    double __P__I_syn_in__X__inh_spikes__d__I_syn_in__X__inh_spikes__d;
    double __P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes;
    double __P__I_syn_ex__X__exc_spikes__I_syn_ex__X__exc_spikes__d;
    double __P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes;
    double __P__I_syn_ex__X__exc_spikes__d__I_syn_ex__X__exc_spikes__d;
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
    Buffers_(hh_moto_5ht3_nestml &);
    Buffers_(const Buffers_ &, hh_moto_5ht3_nestml &);

    /**
     * Logger for all analog data
    **/
    nest::UniversalDataLogger<hh_moto_5ht3_nestml> logger_;

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
    return S_.ode_state[State_::I_syn_ex__X__exc_spikes] * 1.0 * (S_.ode_state[State_::V_m] - P_.E_exc);
  }

  inline double get_I_syn_inh() const
  {
    return S_.ode_state[State_::I_syn_in__X__inh_spikes] * 1.0 * (S_.ode_state[State_::V_m] - P_.E_inh);
  }

  inline double get_E_Ca() const
  {
    return ((1000.0 * P_.R_const * P_.T_current) / (2.0 * P_.F_const)) * std::log10(P_.Ca_out / S_.ode_state[State_::Ca_in]) * 1.0;
  }

  inline double get_I_Na() const
  {
    return P_.g_Na * S_.ode_state[State_::Act_m] * S_.ode_state[State_::Act_m] * S_.ode_state[State_::Act_m] * S_.ode_state[State_::Act_h] * (S_.ode_state[State_::V_m] - P_.E_Na);
  }

  inline double get_I_K() const
  {
    return P_.g_K_rect * S_.ode_state[State_::Inact_n] * S_.ode_state[State_::Inact_n] * S_.ode_state[State_::Inact_n] * S_.ode_state[State_::Inact_n] * (S_.ode_state[State_::V_m] - P_.E_K);
  }

  inline double get_I_L() const
  {
    return P_.g_L * (S_.ode_state[State_::V_m] - P_.E_L);
  }

  inline double get_I_Ca_N() const
  {
    return P_.g_Ca_N * S_.ode_state[State_::Act_mc] * S_.ode_state[State_::Act_mc] * S_.ode_state[State_::Act_hc] * (S_.ode_state[State_::V_m] - (((1000.0 * P_.R_const * P_.T_current) / (2.0 * P_.F_const)) * std::log10(P_.Ca_out / S_.ode_state[State_::Ca_in]) * 1.0));
  }

  inline double get_I_Ca_L() const
  {
    return P_.g_Ca_L * S_.ode_state[State_::Act_p] * (S_.ode_state[State_::V_m] - (((1000.0 * P_.R_const * P_.T_current) / (2.0 * P_.F_const)) * std::log10(P_.Ca_out / S_.ode_state[State_::Ca_in]) * 1.0));
  }

  inline double get_I_K_Ca() const
  {
    return P_.g_K_Ca_5ht * P_.g_K_Ca * ((S_.ode_state[State_::Ca_in] * S_.ode_state[State_::Ca_in]) / (S_.ode_state[State_::Ca_in] * S_.ode_state[State_::Ca_in] + 0.014 * 0.014)) * (S_.ode_state[State_::V_m] - P_.E_K);
  }

  inline double get_I_NaP() const
  {
    return P_.g_NaP * S_.ode_state[State_::Actm_NaP] * S_.ode_state[State_::Inacth_NaP] * (S_.ode_state[State_::V_m] - P_.E_Na);
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
  double h_inf ( double V_m) const;

  //
  double h_tau ( double V_m) const;

  //
  double p_inf ( double V_m) const;

  //
  double mc_inf ( double V_m) const;

  //
  double hc_inf ( double V_m) const;

  //
  double n_inf ( double V_m) const;

  //
  double n_tau ( double V_m) const;

  //
  double alpha_m ( double V_m) const;

  //
  double beta_m ( double V_m) const;

  //
  double m_infNaP ( double V_m) const;

  //
  double Tau_m ( double V_m) const;

  //
  double h_infNaP ( double V_m) const;

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
  static nest::RecordablesMap<hh_moto_5ht3_nestml> recordablesMap_;
  friend int hh_moto_5ht3_nestml_dynamics( double, const double y[], double f[], void* pnode );


}; /* neuron hh_moto_5ht3_nestml */

inline nest_port_t hh_moto_5ht3_nestml::send_test_event(nest::Node& target, nest_rport_t receptor_type, nest::synindex, bool)
{
  // You should usually not change the code in this function.
  // It confirms that the target of connection @c c accepts @c nest::SpikeEvent on
  // the given @c receptor_type.
  nest::SpikeEvent e;
  e.set_sender(*this);
  return target.handles_test_event(e, receptor_type);
}

inline nest_port_t hh_moto_5ht3_nestml::handles_test_event(nest::SpikeEvent&, nest_port_t receptor_type)
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

inline nest_port_t hh_moto_5ht3_nestml::handles_test_event(nest::CurrentEvent&, nest_port_t receptor_type)
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

inline nest_port_t hh_moto_5ht3_nestml::handles_test_event(nest::DataLoggingRequest& dlr, nest_port_t receptor_type)
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

inline void hh_moto_5ht3_nestml::get_status(DictionaryDatum &__d) const
{
  // parameters
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_t_ref, get_t_ref());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_Na, get_g_Na());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_L, get_g_L());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_K_rect, get_g_K_rect());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_Ca_N, get_g_Ca_N());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_Ca_L, get_g_Ca_L());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_K_Ca, get_g_K_Ca());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_K_Ca_5ht, get_g_K_Ca_5ht());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_NaP, get_g_NaP());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Ca_in_init, get_Ca_in_init());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Ca_out, get_Ca_out());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_C_m, get_C_m());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_Na, get_E_Na());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_K, get_E_K());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_L, get_E_L());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_R_const, get_R_const());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_F_const, get_F_const());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_T_current, get_T_current());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_tau_syn_ex, get_tau_syn_ex());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_tau_syn_in, get_tau_syn_in());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_e, get_I_e());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_V_m_init, get_V_m_init());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_hc_tau, get_hc_tau());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_mc_tau, get_mc_tau());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_p_tau, get_p_tau());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_alpha, get_alpha());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_exc, get_E_exc());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_inh, get_E_inh());

  // initial values for state variables in ODE or kernel
  def<long>(__d, nest::hh_moto_5ht3_nestml_names::_r, get_r());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_V_m, get_V_m());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Ca_in, get_Ca_in());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_m, get_Act_m());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_h, get_Act_h());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Inact_n, get_Inact_n());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_p, get_Act_p());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_mc, get_Act_mc());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_hc, get_Act_hc());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Actm_NaP, get_Actm_NaP());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_Inacth_NaP, get_Inacth_NaP());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_in__X__inh_spikes, get_I_syn_in__X__inh_spikes());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_in__X__inh_spikes__d, get_I_syn_in__X__inh_spikes__d());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_ex__X__exc_spikes, get_I_syn_ex__X__exc_spikes());
  def<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_ex__X__exc_spikes__d, get_I_syn_ex__X__exc_spikes__d());

  ArchivingNode::get_status( __d );

  (*__d)[nest::names::recordables] = recordablesMap_.get_list();
  def< double >(__d, nest::names::gsl_error_tol, P_.__gsl_error_tol);
  if ( P_.__gsl_error_tol <= 0. ){
    throw nest::BadProperty( "The gsl_error_tol must be strictly positive." );
  }
}

inline void hh_moto_5ht3_nestml::set_status(const DictionaryDatum &__d)
{
  // parameters
  double tmp_t_ref = get_t_ref();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_t_ref, tmp_t_ref, this);
  double tmp_g_Na = get_g_Na();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_Na, tmp_g_Na, this);
  double tmp_g_L = get_g_L();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_L, tmp_g_L, this);
  double tmp_g_K_rect = get_g_K_rect();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_K_rect, tmp_g_K_rect, this);
  double tmp_g_Ca_N = get_g_Ca_N();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_Ca_N, tmp_g_Ca_N, this);
  double tmp_g_Ca_L = get_g_Ca_L();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_Ca_L, tmp_g_Ca_L, this);
  double tmp_g_K_Ca = get_g_K_Ca();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_K_Ca, tmp_g_K_Ca, this);
  double tmp_g_K_Ca_5ht = get_g_K_Ca_5ht();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_K_Ca_5ht, tmp_g_K_Ca_5ht, this);
  double tmp_g_NaP = get_g_NaP();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_g_NaP, tmp_g_NaP, this);
  double tmp_Ca_in_init = get_Ca_in_init();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Ca_in_init, tmp_Ca_in_init, this);
  double tmp_Ca_out = get_Ca_out();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Ca_out, tmp_Ca_out, this);
  double tmp_C_m = get_C_m();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_C_m, tmp_C_m, this);
  double tmp_E_Na = get_E_Na();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_Na, tmp_E_Na, this);
  double tmp_E_K = get_E_K();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_K, tmp_E_K, this);
  double tmp_E_L = get_E_L();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_L, tmp_E_L, this);
  double tmp_R_const = get_R_const();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_R_const, tmp_R_const, this);
  double tmp_F_const = get_F_const();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_F_const, tmp_F_const, this);
  double tmp_T_current = get_T_current();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_T_current, tmp_T_current, this);
  double tmp_tau_syn_ex = get_tau_syn_ex();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_tau_syn_ex, tmp_tau_syn_ex, this);
  double tmp_tau_syn_in = get_tau_syn_in();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_tau_syn_in, tmp_tau_syn_in, this);
  double tmp_I_e = get_I_e();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_e, tmp_I_e, this);
  double tmp_V_m_init = get_V_m_init();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_V_m_init, tmp_V_m_init, this);
  double tmp_hc_tau = get_hc_tau();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_hc_tau, tmp_hc_tau, this);
  double tmp_mc_tau = get_mc_tau();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_mc_tau, tmp_mc_tau, this);
  double tmp_p_tau = get_p_tau();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_p_tau, tmp_p_tau, this);
  double tmp_alpha = get_alpha();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_alpha, tmp_alpha, this);
  double tmp_E_exc = get_E_exc();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_exc, tmp_E_exc, this);
  double tmp_E_inh = get_E_inh();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_E_inh, tmp_E_inh, this);

  // initial values for state variables in ODE or kernel
  long tmp_r = get_r();
  nest::updateValueParam<long>(__d, nest::hh_moto_5ht3_nestml_names::_r, tmp_r, this);
  double tmp_V_m = get_V_m();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_V_m, tmp_V_m, this);
  double tmp_Ca_in = get_Ca_in();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Ca_in, tmp_Ca_in, this);
  double tmp_Act_m = get_Act_m();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_m, tmp_Act_m, this);
  double tmp_Act_h = get_Act_h();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_h, tmp_Act_h, this);
  double tmp_Inact_n = get_Inact_n();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Inact_n, tmp_Inact_n, this);
  double tmp_Act_p = get_Act_p();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_p, tmp_Act_p, this);
  double tmp_Act_mc = get_Act_mc();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_mc, tmp_Act_mc, this);
  double tmp_Act_hc = get_Act_hc();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Act_hc, tmp_Act_hc, this);
  double tmp_Actm_NaP = get_Actm_NaP();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Actm_NaP, tmp_Actm_NaP, this);
  double tmp_Inacth_NaP = get_Inacth_NaP();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_Inacth_NaP, tmp_Inacth_NaP, this);
  double tmp_I_syn_in__X__inh_spikes = get_I_syn_in__X__inh_spikes();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_in__X__inh_spikes, tmp_I_syn_in__X__inh_spikes, this);
  double tmp_I_syn_in__X__inh_spikes__d = get_I_syn_in__X__inh_spikes__d();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_in__X__inh_spikes__d, tmp_I_syn_in__X__inh_spikes__d, this);
  double tmp_I_syn_ex__X__exc_spikes = get_I_syn_ex__X__exc_spikes();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_ex__X__exc_spikes, tmp_I_syn_ex__X__exc_spikes, this);
  double tmp_I_syn_ex__X__exc_spikes__d = get_I_syn_ex__X__exc_spikes__d();
  nest::updateValueParam<double>(__d, nest::hh_moto_5ht3_nestml_names::_I_syn_ex__X__exc_spikes__d, tmp_I_syn_ex__X__exc_spikes__d, this);

  // We now know that (ptmp, stmp) are consistent. We do not
  // write them back to (P_, S_) before we are also sure that
  // the properties to be set in the parent class are internally
  // consistent.
  ArchivingNode::set_status(__d);

  // if we get here, temporaries contain consistent set of properties
  set_t_ref(tmp_t_ref);
  set_g_Na(tmp_g_Na);
  set_g_L(tmp_g_L);
  set_g_K_rect(tmp_g_K_rect);
  set_g_Ca_N(tmp_g_Ca_N);
  set_g_Ca_L(tmp_g_Ca_L);
  set_g_K_Ca(tmp_g_K_Ca);
  set_g_K_Ca_5ht(tmp_g_K_Ca_5ht);
  set_g_NaP(tmp_g_NaP);
  set_Ca_in_init(tmp_Ca_in_init);
  set_Ca_out(tmp_Ca_out);
  set_C_m(tmp_C_m);
  set_E_Na(tmp_E_Na);
  set_E_K(tmp_E_K);
  set_E_L(tmp_E_L);
  set_R_const(tmp_R_const);
  set_F_const(tmp_F_const);
  set_T_current(tmp_T_current);
  set_tau_syn_ex(tmp_tau_syn_ex);
  set_tau_syn_in(tmp_tau_syn_in);
  set_I_e(tmp_I_e);
  set_V_m_init(tmp_V_m_init);
  set_hc_tau(tmp_hc_tau);
  set_mc_tau(tmp_mc_tau);
  set_p_tau(tmp_p_tau);
  set_alpha(tmp_alpha);
  set_E_exc(tmp_E_exc);
  set_E_inh(tmp_E_inh);
  set_r(tmp_r);
  set_V_m(tmp_V_m);
  set_Ca_in(tmp_Ca_in);
  set_Act_m(tmp_Act_m);
  set_Act_h(tmp_Act_h);
  set_Inact_n(tmp_Inact_n);
  set_Act_p(tmp_Act_p);
  set_Act_mc(tmp_Act_mc);
  set_Act_hc(tmp_Act_hc);
  set_Actm_NaP(tmp_Actm_NaP);
  set_Inacth_NaP(tmp_Inacth_NaP);
  set_I_syn_in__X__inh_spikes(tmp_I_syn_in__X__inh_spikes);
  set_I_syn_in__X__inh_spikes__d(tmp_I_syn_in__X__inh_spikes__d);
  set_I_syn_ex__X__exc_spikes(tmp_I_syn_ex__X__exc_spikes);
  set_I_syn_ex__X__exc_spikes__d(tmp_I_syn_ex__X__exc_spikes__d);




  updateValue< double >(__d, nest::names::gsl_error_tol, P_.__gsl_error_tol);
  if ( P_.__gsl_error_tol <= 0. )
  {
    throw nest::BadProperty( "The gsl_error_tol must be strictly positive." );
  }

  // recompute internal variables in case they are dependent on parameters or state that might have been updated in this call to set_status()
  recompute_internal_variables();
};



#endif /* #ifndef HH_MOTO_5HT3_NESTML */
