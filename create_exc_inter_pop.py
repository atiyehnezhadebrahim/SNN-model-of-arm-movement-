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

class create_exc_inter_population():
    def __init__(self):
        self.senders = []
        self.spiketimes = []
        self.saved_spiketimes = []
        self.saved_senders = []
        self.time_window = 50		#50*0.1=5ms time window, based on time resolution of 0.1
        self.count = 0
        
        #Create population
        self.bursting_neuronparams = {'C_m':nest.random.normal(mean=netparams.C_m_bursting_mean, std=netparams.C_m_bursting_std), 'g_L':26.,'E_L':-60.,'V_th':nest.random.normal(mean=netparams.V_th_mean, std=netparams.V_th_std),'Delta_T':2.,'tau_w':130., 'a':-11., 'b':30., 'V_reset':-48., 'I_e':nest.random.normal(mean=netparams.I_e_bursting_mean, std=netparams.I_e_bursting_std),'t_ref':nest.random.normal(mean=netparams.t_ref_mean, std=netparams.t_ref_std),'V_m':nest.random.normal(mean=netparams.V_m_mean, std=netparams.V_m_std)} #bursting, Naud et al. 2008, C = pF; g_L = nS
        self.tonic_neuronparams = {'C_m':nest.random.normal(mean=netparams.C_m_tonic_mean, std=netparams.C_m_tonic_std), 'g_L':10.,'E_L':-70.,'V_th':-50.,'Delta_T':2.,'tau_w':30., 'a':3., 'b':0., 'V_reset':-58., 'I_e':nest.random.normal(mean=netparams.I_e_tonic_mean, std=netparams.I_e_tonic_std),'t_ref':nest.random.normal(mean=netparams.t_ref_mean, std=netparams.t_ref_std),'V_m':nest.random.normal(mean=netparams.V_m_mean, std=netparams.V_m_std)}

        #self.exc_inter_tonic = nest.Create('aeif_cond_alpha',netparams.num_exc_inter_tonic,self.tonic_neuronparams)
        #self.exc_inter_bursting = nest.Create('aeif_cond_alpha',netparams.num_exc_inter_bursting,self.bursting_neuronparams)		
        self.hh_exc_int_neuronparams = {'I_e':0.000}
       
        self.exc_inter_pop = nest.Create('aeif_cond_alpha',netparams.num_exc_inter_pop,self.hh_exc_int_neuronparams)
        #Create noise
        self.white_noise_tonic = nest.Create("noise_generator",netparams.noise_params_tonic)
        #self.white_noise_bursting = nest.Create("noise_generator",netparams.noise_params_bursting)
        
        #Create spike detectors (for recording spikes)
        self.spike_detector_exc_inter_tonic = nest.Create("spike_recorder",netparams.num_exc_inter_pop)
        #self.spike_detector_exc_inter_bursting = nest.Create("spike_recorder",netparams.num_exc_inter_bursting)


        
        #Create multimeters (for recording membrane potential)
        self.mm_exc_inter_tonic = nest.Create("multimeter",netparams.mm_params)
        #self.mm_exc_inter_bursting = nest.Create("multimeter",netparams.mm_params)
	
        #Connect white noise to neurons
        nest.Connect(self.white_noise_tonic, self.exc_inter_pop,"all_to_all")
        #nest.Connect(self.white_noise_bursting,self.exc_inter_bursting,"all_to_all")

        #Connect spike detectors to neuron populations
        nest.Connect( self.exc_inter_pop,self.spike_detector_exc_inter_tonic,"one_to_one")
        #nest.Connect(self.exc_inter_bursting,self.spike_detector_exc_inter_bursting,"one_to_one")

        #Connect multimeters to neuron populations
        nest.Connect(self.mm_exc_inter_tonic, self.exc_inter_pop)
        #nest.Connect(self.mm_exc_inter_bursting,self.exc_inter_bursting)
        
        #Connect neurons within exc interneuron pop
        '''
        self.inh_syn_params = {"synapse_model":"static_synapse",
            "weight" : nest.random.normal(mean=netparams.w_inh_mean,std=netparams.w_inh_std), #nS            
            "delay" : netparams.synaptic_delay}	#ms
        self.exc_syn_params = {"synapse_model":"static_synapse",
            "weight" : nest.random.normal(mean=netparams.w_exc_mean,std=netparams.w_exc_std), #nS
            "delay" : netparams.synaptic_delay}	#ms
        nest.Connect(self.exc_inter_tonic,self.exc_inter_tonic,'all_to_all',self.exc_syn_params)
        nest.Connect(self.exc_inter_tonic,self.exc_inter_bursting,'all_to_all',self.exc_syn_params)
        nest.Connect(self.exc_inter_bursting,self.exc_inter_bursting,'all_to_all',self.exc_syn_params)
        nest.Connect(self.exc_inter_bursting,self.exc_inter_tonic,'all_to_all',self.exc_syn_params)	
                
        '''