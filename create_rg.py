#!/usr/bin/env python

#include <static_connection.h>
import nest
import nest.raster_plot
import numpy as np
import sys
import pylab
import math
import matplotlib.pyplot as pyplot
import pickle, yaml
import random
import scipy
import scipy.fftpack
from scipy.ndimage import gaussian_filter
from scipy.signal import find_peaks, peak_widths, peak_prominences
import time
import copy
from set_network_params import neural_network
netparams = neural_network()

class create_rg_population():
	def __init__(self):
		self.senders = []
		self.spiketimes = []
		self.saved_spiketimes = []
		self.saved_senders = []
		self.time_window = 50		#50*0.1=5ms time window, based on time resolution of 0.1
		self.count = 0
		
		
		#Create populations for rg
		# self.bursting_neuronparams = {'C_m':nest.random.normal(mean=netparams.C_m_bursting_mean, std=netparams.C_m_bursting_std), 'g_L':26.,'E_L':-60.,'V_th':nest.random.normal(mean=netparams.V_th_mean, std=netparams.V_th_std),'Delta_T':2.,'tau_w':130., 'a':-11., 'b':30., 'V_reset':-48., 'I_e':nest.random.normal(mean=netparams.I_e_bursting_mean, std=netparams.I_e_bursting_std),'t_ref':nest.random.normal(mean=netparams.t_ref_mean, std=netparams.t_ref_std),'V_m':nest.random.normal(mean=netparams.V_m_mean, std=netparams.V_m_std)} #bursting, Naud et al. 2008, C = pF; g_L = nS
		# self.tonic_neuronparams = {'C_m':nest.random.normal(mean=netparams.C_m_tonic_mean, std=netparams.C_m_tonic_std), 'g_L':10.,'E_L':-70.,'V_th':-50.,'Delta_T':2.,'tau_w':30., 'a':3., 'b':0., 'V_reset':-58., 'I_e':nest.random.normal(mean=netparams.I_e_tonic_mean, std=netparams.I_e_tonic_std),'t_ref':nest.random.normal(mean=netparams.t_ref_mean, std=netparams.t_ref_std),'V_m':nest.random.normal(mean=netparams.V_m_mean, std=netparams.V_m_std)}
		

		self.rg_exc_neuronparams = {'I_e':0.0}
		self.rg_exc_pop = nest.Create('aeif_cond_alpha',netparams.rg_pop_neurons,self.rg_exc_neuronparams)
		# self.rg_inh_bursting = nest.Create('aeif_cond_alpha',netparams.inh_bursting_count,self.bursting_neuronparams)	
		# if netparams.exc_tonic_count != 0: self.rg_exc_tonic = nest.Create('aeif_cond_alpha',netparams.exc_tonic_count,self.tonic_neuronparams) 	
		# if netparams.inh_tonic_count != 0: self.rg_inh_tonic = nest.Create('aeif_cond_alpha',netparams.inh_tonic_count,self.tonic_neuronparams) 
		
		#Create noise
		self.white_noise_rg_exc = nest.Create("noise_generator",netparams.noise_params_tonic) 
		#self.white_noise_bursting = nest.Create("noise_generator",netparams.noise_params_bursting)   
		# DC Genrator 
		self.dc_generator_rg_exc = nest.Create("dc_generator",netparams.rg_pop_neurons)  
		self.dc_generator_rg_exc.set(amplitude=800, start=1, stop=3000)
		
		#Create spike detectors (for recording spikes)
		self.spike_detector_rg_exc = nest.Create("spike_recorder",netparams.rg_pop_neurons)
		# self.spike_detector_rg_inh_bursting = nest.Create("spike_recorder",netparams.inh_bursting_count)
		# if netparams.exc_tonic_count != 0: 
		#     self.spike_detector_rg_exc_tonic = nest.Create("spike_recorder",netparams.exc_tonic_count)
		# if netparams.inh_tonic_count != 0: 
		#     self.spike_detector_rg_inh_tonic = nest.Create("spike_recorder",netparams.inh_tonic_count)
				
		#Create multimeters (for recording membrane potential)
		self.mm_rg_exc = nest.Create("multimeter",netparams.mm_params)
		# self.mm_rg_inh_bursting = nest.Create("multimeter",netparams.mm_params)
		# self.mm_rg_exc_tonic = nest.Create("multimeter",netparams.mm_params)
		# self.mm_rg_inh_tonic = nest.Create("multimeter",netparams.mm_params)
	    
		#Connect white noise to neurons
		nest.Connect(self.white_noise_rg_exc,self.rg_exc_pop,"all_to_all")
		# nest.Connect(self.white_noise_bursting,self.rg_inh_bursting,"all_to_all")
		# if netparams.exc_tonic_count != 0: nest.Connect(self.white_noise_tonic,self.rg_exc_tonic,"all_to_all") 
		# if netparams.inh_tonic_count != 0: nest.Connect(self.white_noise_tonic,self.rg_inh_tonic,"all_to_all") 
		 #connect DC generator to neurons
		nest.Connect(self.dc_generator_rg_exc,self.rg_exc_pop,'one_to_one')
 
		#Connect spike detectors to neuron populations
		nest.Connect(self.rg_exc_pop,self.spike_detector_rg_exc,"one_to_one")
					
		#Connect multimeters to neuron populations
		nest.Connect(self.mm_rg_exc,self.rg_exc_pop)
	
rg = create_rg_population()       	        
