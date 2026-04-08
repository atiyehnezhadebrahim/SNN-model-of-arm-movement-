#!/usr/bin/env python

#include <static_connection.h>
import nest
import nest.raster_plot
import numpy as np
import sys
import pylab
import math
import matplotlib.pyplot as plt
import pickle, yaml
import random
import scipy
import scipy.fftpack
from scipy.signal import find_peaks, peak_widths, peak_prominences
import time
import numpy as np
import copy
from set_network_params import neural_network
netparams = neural_network()

class create_mnp():
    def __init__(self):
        self.senders = []
        self.spiketimes = []
        self.saved_spiketimes = []
        self.saved_senders = []
        self.time_window = 50		#50*0.1=5ms time window, based on time resolution of 0.1
        self.count = 0
        #Define tonic dopamine input 
        self.tonic_dopa_rate = netparams.tonic_dopa_rate  # s^-1 (Cohen et al., 2012)
        self.spike_interval = 1000*(1/self.tonic_dopa_rate) # ms
        self.tonic_spike_times = np.arange(1,netparams.sim_time,self.spike_interval)
        self.tonic_noise = np.random.normal(0, self.spike_interval/5, len(self.tonic_spike_times))
        self.tonic_spike_times = self.tonic_spike_times[1:]+self.tonic_noise[1:]
        self.tonic_spike_times  = np.round(self.tonic_spike_times, 1)
        self.tonic_spike_times = np.sort(self.tonic_spike_times)
        print(self.tonic_spike_times)
        #Create population
        #self.bursting_neuronparams = {'C_m':nest.random.normal(mean=netparams.C_m_bursting_mean, std=netparams.C_m_bursting_std), 'g_L':26.,'E_L':-60.,'V_th':nest.random.normal(mean=netparams.V_th_mean, std=netparams.V_th_std),'Delta_T':2.,'tau_w':130., 'a':-11., 'b':30., 'V_reset':-48., 'I_e':nest.random.normal(mean=netparams.I_e_bursting_mean, std=netparams.I_e_bursting_std),'t_ref':nest.random.normal(mean=netparams.t_ref_mean, std=netparams.t_ref_std),'V_m':nest.random.normal(mean=netparams.V_m_mean, std=netparams.V_m_std)} #bursting, Naud et al. 2008, C = pF; g_L = nS
        #self.tonic_neuronparams = {'C_m':nest.random.normal(mean=netparams.C_m_tonic_mean, std=netparams.C_m_tonic_std), 'g_L':10.,'E_L':-70.,'V_th':-50.,'Delta_T':2.,'tau_w':30., 'a':3., 'b':0., 'V_reset':-58., 'I_e':nest.random.normal(mean=netparams.I_e_tonic_mean, std=netparams.I_e_tonic_std),'t_ref':nest.random.normal(mean=netparams.t_ref_mean, std=netparams.t_ref_std),'V_m':nest.random.normal(mean=netparams.V_m_mean, std=netparams.V_m_std)}
        #self.tonic_neuronparams = {'C_m':nest.random.normal(mean=netparams.C_m_tonic_mean, std=netparams.C_m_tonic_std), 'g_L':10.,'E_L':-70.,'V_th':-50.,'Delta_T':2.,'tau_w':30., 'a':3., 'b':0., 'V_reset':-58., 'I_e':0.,'t_ref':nest.random.normal(mean=netparams.t_ref_mean, std=netparams.t_ref_std),'V_m':nest.random.normal(mean=netparams.V_m_mean, std=netparams.V_m_std)}
        
        
        self.hh_motor_neuroparams = {'I_e':00}
        
        self.motor_neuron_pop = nest.Create('aeif_cond_alpha',netparams.num_motor_neurons,self.hh_motor_neuroparams)		
        
        #Create noise
        self.white_noise = nest.Create("noise_generator",netparams.noise_params_tonic)
        

        
        #Create spike generators for tonic and phasic dopamine
        self.sg_dopa_tonic = nest.Create("spike_generator", params={"spike_times": self.tonic_spike_times})            
        #self.sg_dopa_phasic = nest.Create("inhomogeneous_poisson_generator", params={"rate_times": self.time_dopa_rates,"rate_values":self.value_dopa_rates})

        #Create parrot neuron to send dopaminergic spikes
        self.vt_parrot = nest.Create("parrot_neuron")

        #Connect dopaminergic spike generators to the parrot neuron
        nest.Connect(self.sg_dopa_tonic, self.vt_parrot) 
        #nest.Connect(self.sg_dopa_phasic, self.vt_parrot)

         #connect parrot neurons to MNP
        nest.Connect(self.vt_parrot, self.motor_neuron_pop,syn_spec={"synapse_model": "static_synapse",
                                  "weight": 0.0,
                                  "delay": 1.} )  

        #Create spike detectors (for recording spikes)
        self.spike_detector_motor = nest.Create("spike_recorder",netparams.num_motor_neurons)

                
        #Create multimeters (for recording membrane potential)
        self.mm_motor = nest.Create("multimeter",netparams.mm_params)
	    
        

        #Connect white noise to neurons
        nest.Connect(self.white_noise,self.motor_neuron_pop,"all_to_all")

        #Connect spike detectors to neuron populations
        nest.Connect(self.motor_neuron_pop,self.spike_detector_motor,"one_to_one")
        self.spike_detector_motor.n_events = 0		#ensure no spikes left from previous simulations

        #Connect multimeters to neuron populations
        nest.Connect(self.mm_motor,self.motor_neuron_pop)
       	        
        