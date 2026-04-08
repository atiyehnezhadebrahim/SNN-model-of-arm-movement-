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
import pywt
import set_network_params as netparams
import random as rnd

nn=netparams.neural_network()

def update_neuronal_characteristic(update_charac,neuron_population,leakage_value):
	neuron_charac = update_charac
	for neuron in neuron_population:
		nest.SetStatus(neuron, {neuron_charac: leakage_value})
	new_val = nest.GetStatus(neuron_population, keys=neuron_charac)[0]
	return new_val

def read_spike_data(spike_detector):
	senders = []
	spiketimes = []
	spike_detector = spike_detector
	senders += [spike_detector.get('events', 'senders')]
	spiketimes += [spike_detector.get('events', 'times')]
	return senders,spiketimes

def read_membrane_potential(multimeter,pop_size):
	neuron_num=rnd.randint(1,pop_size)
	mm = nest.GetStatus(multimeter,keys="events")[0]
	V_m =  mm['V_m']
	t_vm = mm['times']
	Inacth_NaP = mm['Inacth_NaP']
	V_m = V_m[neuron_num]
	Inacth_NaP = Inacth_NaP[neuron_num]
	t_vm = t_vm[neuron_num]
	return V_m,t_vm,Inacth_NaP

def read_membrane_potentiall(multimeter,pop_size,neuron_num):
	mm = nest.GetStatus(multimeter,keys="events")[0]
	vm =  mm['V_m']
	t_vm = mm['times']
	Inacth_NaP = mm['Inacth_NaP']
	I_syn_exc = mm['I_syn_exc']
	I_syn_inh = mm['I_syn_inh']
	vm = vm[neuron_num::pop_size]
	t_vm = t_vm[neuron_num::pop_size]
	Inacth_NaP =Inacth_NaP[neuron_num::pop_size]
	I_syn_exc =I_syn_exc[neuron_num::pop_size]
	I_syn_inh =I_syn_inh[neuron_num::pop_size]
	
	return vm,t_vm,Inacth_NaP,I_syn_inh,I_syn_exc

def count_indiv_spikes(total_neurons,neuron_id_data):
		spike_count_array = [len(neuron_id_data[0][i]) for i in range(total_neurons)]
		sparse_count_max = 4*(nn.sim_time/1000)	#Spiking 2 times per period considered sparse
		sparse_firing_count = [i for i, count in enumerate(spike_count_array) if count>=1 and count<=sparse_count_max]
		silent_neuron_count = [i for i, count in enumerate(spike_count_array) if count==0]
		neuron_to_sample = sparse_firing_count[1] if len(sparse_firing_count) > 1 else 0
		#print('Max for sparse firing: ',sparse_count_max)
		return spike_count_array,neuron_to_sample,len(sparse_firing_count),len(silent_neuron_count) 

def save_spike_data(num_neurons,population,neuron_num_offset):
	spike_time = []
	all_spikes = []
	for i in range(num_neurons):
		spike_data = population[0][i]
		neuron_num = [i+neuron_num_offset]*spike_data.shape[0]
		for j in range(spike_data.shape[0]):
			spike_time.append(spike_data[j])    
		indiv_spikes = list(zip(neuron_num,spike_time))
		all_spikes.extend(indiv_spikes)  
		spike_time = []     
	return all_spikes

def single_neuron_spikes(neuron_number,population):
	spike_time = [0]*int(nn.sim_time/nn.time_resolution)
	spike_data = population[0][neuron_number]
	for j in range(spike_data.shape[0]):
		spike_time_index = int(spike_data[j]*(1/nn.time_resolution))-1
		spike_time[spike_time_index]=spike_data[j]        
	return spike_time

def single_neuron_spikes_binary(neuron_number,population):
	spike_time = [0]*int(nn.sim_time/nn.time_resolution)
	spike_data = population[0][neuron_number]
	for j in range(spike_data.shape[0]):
		spike_time_index = int(spike_data[j]*(1/nn.time_resolution))-1
		spike_time[spike_time_index]=1        
	return spike_time

def rate_code_spikes(neuron_count, output_spiketimes):
	# Initialize the spike bins array as a 2D array
	bins=np.arange(0, nn.sim_time+nn.time_resolution,nn.time_resolution)
	# Loop over each neuron
	for i in range(neuron_count):
		t_spikes = output_spiketimes[0][i]
		# Use numpy's histogram function to assign each spike to its corresponding time bin index
		spikes_per_bin,bin_edges=np.histogram(t_spikes, bins)
		# Add the spike counts to the `spike_bins_current` array
		if i == 0:
			spike_bins_current = spikes_per_bin
		else:
			spike_bins_current += spikes_per_bin
	spike_bins_current = sliding_time_window(spike_bins_current,nn.time_window) #Applies a time window to smooth the output        
	smoothed_spike_bins = gaussian_filter(spike_bins_current, nn.convstd_rate) #Applies a filter to smooth the high frequency noise
	if nn.chop_edges_amount > 0.0:
		smoothed_spike_bins = smoothed_spike_bins[int(nn.chop_edges_amount):]
	return smoothed_spike_bins

def sliding_time_window(signal, window_size):
	windows = np.lib.stride_tricks.sliding_window_view(signal, window_size)
	return np.sum(windows, axis=1)

def sliding_time_window_matrix(signal, window_size):
	result = []
	for row in signal:
		windows = np.lib.stride_tricks.sliding_window_view(row, window_size)
		row_sum = np.sum(windows, axis=1)
		result.append(row_sum)
	return np.array(result)

def smooth(data, sd):
	data = copy.copy(data)       
	from scipy.signal import gaussian
	from scipy.signal import convolve
	n_bins = data.shape[1]
	w = n_bins - 1 if n_bins % 2 == 0 else n_bins
	window = gaussian(w, std=sd)
	for j in range(data.shape[0]):
		data[j,:] = convolve(data[j,:], window, mode='same', method='auto') 
	return data

def convolve_spiking_activity(population_size,population):
	time_steps = int(nn.sim_time/nn.time_resolution) 
	binary_spikes = np.vstack([single_neuron_spikes_binary(i, population) for i in range(population_size)])
	binned_spikes = sliding_time_window_matrix(binary_spikes,nn.time_window)
	smoothed_spikes = smooth(binned_spikes, nn.convstd_pca)
	#smoothed_spikes = smooth(binary_spikes, nn.convstd_pca)
	if nn.chop_edges_amount > 0.0:
		smoothed_spikes = smoothed_spikes[:,int(nn.chop_edges_amount):int(-nn.chop_edges_amount)]
	if nn.remove_mean:
		smoothed_spikes = (smoothed_spikes.T - np.mean(smoothed_spikes, axis=1)).T
	if nn.high_pass_filtered:
		#print('High pass filtering the output.')            
		from scipy.signal import butter, sosfilt, filtfilt, sosfiltfilt
		# Same used as in Linden et al, 2022 paper
		b, a = butter(3, .1, 'highpass', fs=1000)		#high pass freq was previously 0.3Hz
		smoothed_spikes = filtfilt(b, a, smoothed_spikes)
	if nn.downsampling_convolved:
		from scipy.signal import decimate
		smoothed_spikes = decimate(smoothed_spikes, int(1/nn.time_resolution), n=2, ftype='iir', zero_phase=True)
	smoothed_spikes = smoothed_spikes[:, :-nn.time_window+1] #truncate array by the width of the time window
	return smoothed_spikes

def inject_current(neuron_population,current):
	for neuron in neuron_population:
		nest.SetStatus([neuron],{"I_e": current})
	updated_current = nest.GetStatus(neuron_population, keys="I_e")[0]
	return updated_current
	
def normalize_rows(matrix):
	max_values = np.max(matrix, axis=1, keepdims=True)
	normalized_matrix = matrix / max_values
	return normalized_matrix	


def rate_code_spikes_nofilter(neuron_count, output_spiketimes):
	# Initialize the spike bins array as a 2D array
	bins=np.arange(0, nn.sim_time+nn.time_resolution,nn.time_resolution)
	# Loop over each neuron
	for i in range(neuron_count):
		t_spikes = output_spiketimes[0][i]
		# Use numpy's histogram function to assign each spike to its corresponding time bin index
		spikes_per_bin,bins=np.histogram(t_spikes, bins)
		# Add the spike counts to the `spike_bins_current` array
		if i == 0:
			spike_bins_current = spikes_per_bin
		else:
			spike_bins_current += spikes_per_bin
	spike_bins_current = sliding_time_window(spike_bins_current,nn.time_window) #Applies a time window to smooth the output        
	smoothed_spike_bins = gaussian_filter(spike_bins_current, nn.convstd_rate) #Applies a filter to smooth the high frequency noise
	#if nn.chop_edges_amount > 0.0:
		#smoothed_spike_bins = smoothed_spike_bins[int(nn.chop_edges_amount):int(-nn.chop_edges_amount)]
	return smoothed_spike_bins,bins


def extract_rat_eng(firings,samplingRate = 1000.,delay_ms=2):
	""" Return the eng activity given the cell firings.

	Keyword arguments:
		firings -- Cell firings, a 2d numpy array (nCells x time).
		samplingRate -- Sampling rate of the extracted signal in Hz (default = 1000).
		delay_ms -- delay in ms between an action potential (AP) and a motor unit
		action potential (MUAP).
	"""
	eng = None
	
	nCells = firings.shape[0]
	print('nCells',nCells)
	nSamples = firings.shape[1]
	print('nSamples',nSamples)

	dt = 1000./samplingRate
	delay = int(delay_ms/dt)

		# MUAP duration between 400-410ms 
	meanLenMUAP = int(250/dt)
	stdLenMUAP = int(1/dt)
	nS = [int(meanLenMUAP+rnd.gauss(0,stdLenMUAP)) for i in range(firings.shape[0])]  #Ã¿žöcell³ÖÐøµÄÊ±³€

	
	Amp = [abs(1+rnd.gauss(0,0.2)) for i in range(firings.shape[0])]
	
	eng = np.zeros(nSamples + max(nS)+delay)
	
		# create MUAP shape
	for i in range(nCells):
		n40perc = int(nS[i]*0.4)
		
		n60perc = nS[i]-n40perc
		
		amplitudeMod = (1-(np.linspace(0,1,nS[i])**2)) * np.concatenate((np.ones(n40perc),1/np.linspace(1,3,n60perc)))
		
		logBase = 1.05
		freqMod = np.log(np.linspace(1,logBase**(4*np.pi),nS[i]))/np.log(logBase)
		
		eng_unit = Amp[i]*amplitudeMod*np.sin(freqMod)
		
		for j in range(nSamples):
			if firings[i,j]==1:
				eng[j+delay:j+delay+nS[i]]=eng[j+delay:j+delay+nS[i]]+eng_unit
	eng = eng[:nSamples]
	print('eng',eng)
	return eng

def rate_code_spikess(neuron_count, output_spiketimes):
	# Initialize the spike bins array as a 2D array
	bins=np.arange(0, nn.sim_time+nn.time_resolution,nn.time_resolution)
	# Loop over each neuron
	for i in range(neuron_count):
		t_spikes = output_spiketimes[0][i]
		# Use numpy's histogram function to assign each spike to its corresponding time bin index
		spikes_per_bin,bin_edges=np.histogram(t_spikes, bins)
		# Add the spike counts to the `spike_bins_current` array
		if i == 0:
			spike_bins_current = spikes_per_bin
		else:
			spike_bins_current += spikes_per_bin
	spike_bins_current = sliding_time_window(spike_bins_current,nn.time_window) #Applies a time window to smooth the output        
	# smoothed_spike_bins = gaussian_filter(spike_bins_current, nn.convstd_rate) #Applies a filter to smooth the high frequency noise
	# if nn.chop_edges_amount > 0.0:
	# 	smoothed_spike_bins = smoothed_spike_bins[int(nn.chop_edges_amount):int(-nn.chop_edges_amount)]
	return spike_bins_current

# function to extract times array for ENG (extract spikes time and put them in a 2d array)
def ENG_structure(neuron_number,spike_detector):
	ENG_array=[]
	for i in range(neuron_number):
		times=nest.GetStatus(spike_detector)[i]["events"]["times"]
		ENG_array.append(times)
		
	max_spikes = max(len(spikes) for spikes in ENG_array)
	spike_matrix = []

	for spikes in ENG_array:
		padded_spikes = np.pad(spikes, (0, max_spikes - len(spikes)), constant_values=0)
		spike_matrix.append(padded_spikes)  
	spike_matrix=np.array(spike_matrix)
	
	return spike_matrix
#constant_values=np.nan
'''
def ENG_(spike_matrix):
	wavelet = 'db1'
	wavelet_object = pywt.Wavelet(wavelet)
	wavelet_coefficients = wavelet_object.filter_bank
	scaling_function = wavelet_coefficients[0]
	print('scaling_function:',scaling_function)
	wavelet_function = wavelet_coefficients[1]
	print('wavelet_function:',wavelet_function)
	approximation = scipy.signal.convolve2d(spike_matrix, scaling_function, mode='same')
	print('approximation:',approximation)
	detail = scipy.signal.convolve2d(spike_matrix, wavelet_function, mode='same')
	print('detail:',detail)
	return approximation, detail
'''
#define time-spikes array for neurons (get times of spike and make a 2d array to have time of spiking for each neuron in an array)
def time_spike_array(times_matrix,simulation_time):
	times_matrix = np.round(times_matrix).astype(int)
	simulation_time_steps = simulation_time+1 
	spike_matrix = np.zeros((times_matrix.shape[0], simulation_time_steps), dtype=float)


	for i, spikes in enumerate(times_matrix):
		spike_matrix[i, spikes] = 1

	return spike_matrix

#define eng out put from a mexican hat-like wavelet
