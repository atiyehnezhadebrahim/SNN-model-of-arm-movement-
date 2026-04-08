#!/usr/bin/env python

import nest
import nest.raster_plot
import numpy as np
import sys
import pylab
import math
import matplotlib.pyplot as plt
import random
import time
import start_simulation as ss
import pickle, yaml
import pandas as pd
#import elephant
from scipy.signal import find_peaks,correlate
from scipy.fft import fft, fftfreq
import set_network_params as netparams
from phase_ordering import order_by_phase
from pca import run_PCA
#from gap_junction_functions import gap_pairs
#from dopamine_functions import dopamine_activity
from connect_populations import connect
import population_functions as popfunc
from excitations_file_gen import file_gen

ss.nest_start()


nn=netparams.neural_network()
# gj=gap_pairs()
# dopa=dopamine_activity()
conn=connect() 

import create_rg as rg
import create_inh_pop as inh
import create_exc_inter_pop as exc
import create_mnp as mnp

#Create neuron populations - NEST
rg1 = rg.create_rg_population()
rg2 = rg.create_rg_population()

exc1 = exc.create_exc_inter_population()
exc2 = exc.create_exc_inter_population()

mnp1 = mnp.create_mnp()
mnp2 = mnp.create_mnp()

#Connect rg neurons to excitatory interneuron populations
conn.create_connections(rg1.rg_exc_pop,exc1.exc_inter_pop,'exc_all')

conn.create_connections(rg2.rg_exc_pop,exc2.exc_inter_pop,'exc_all')


#Create self-excitation of excitatory interneuron populations  
''' 
conn.create_connections(exc1.exc_inter_tonic,exc1.exc_inter_tonic,'exc_mn')
conn.create_connections(exc1.exc_inter_tonic,exc1.exc_inter_bursting,'exc_mn')
conn.create_connections(exc1.exc_inter_bursting,exc1.exc_inter_tonic,'exc_mn')
conn.create_connections(exc1.exc_inter_bursting,exc1.exc_inter_bursting,'exc_mn')

conn.create_connections(exc2.exc_inter_tonic,exc2.exc_inter_tonic,'exc_mn')
conn.create_connections(exc2.exc_inter_tonic,exc2.exc_inter_bursting,'exc_mn')
conn.create_connections(exc2.exc_inter_bursting,exc2.exc_inter_tonic,'exc_mn')
conn.create_connections(exc2.exc_inter_bursting,exc2.exc_inter_bursting,'exc_mn')
'''

#Connect excitatory interneuron populations to motor neuron populations
conn.create_connections(exc1.exc_inter_pop,mnp1.motor_neuron_pop,'exc_all')
#conn.create_connections(exc1.exc_inter_bursting,mnp1.motor_neuron_pop,'exc_mn')
conn.create_connections(exc2.exc_inter_pop,mnp2.motor_neuron_pop,'exc_all')
#conn.create_connections(exc2.exc_inter_bursting,mnp2.motor_neuron_pop,'exc_mn')

if nn.rgs_connected==1:
	inh1 = inh.create_inh_population()
	inh2 = inh.create_inh_population()
	
	#Connect excitatory rg neurons to inhibitory populations
	conn.create_connections(rg1.rg_exc_pop,inh1.inh_pop,'exc_all')
	#conn.create_connections(rg1.rg_exc_tonic,inh1.inh_pop,'exc')
	conn.create_connections(rg2.rg_exc_pop,inh2.inh_pop,'exc_all')
	#conn.create_connections(rg2.rg_exc_tonic,inh2.inh_pop,'exc')

	#Connect inhibitory populations to all rg neurons
	#if nn.dopamod_synapses==0:
	conn.create_connections(inh1.inh_pop,rg2.rg_exc_pop,'inh_all')

	conn.create_connections(inh2.inh_pop,rg1.rg_exc_pop,'inh_all')
		
	#Connect inhibitory populations to mnps
	conn.create_connections(inh1.inh_pop,mnp2.motor_neuron_pop,'inh_strong_mn')
	conn.create_connections(inh2.inh_pop,mnp1.motor_neuron_pop,'inh_strong_mn')
	'''
	if nn.dopamod_synapses==1:
		dopa.create_dopa_connections(inh1.inh_pop,rg2.rg_exc_bursting,'inh_dopamod_cpg')
		dopa.create_dopa_connections(inh1.inh_pop,rg2.rg_exc_tonic,'inh_dopamod_cpg')
		dopa.create_dopa_connections(inh1.inh_pop,rg2.rg_inh_bursting,'inh_dopamod_cpg')
		dopa.create_dopa_connections(inh1.inh_pop,rg2.rg_inh_tonic,'inh_dopamod_cpg')

		dopa.create_dopa_connections(inh2.inh_pop,rg1.rg_exc_bursting,'inh_dopamod_cpg')
		dopa.create_dopa_connections(inh2.inh_pop,rg1.rg_exc_tonic,'inh_dopamod_cpg')
		dopa.create_dopa_connections(inh2.inh_pop,rg1.rg_inh_bursting,'inh_dopamod_cpg')
		dopa.create_dopa_connections(inh2.inh_pop,rg1.rg_inh_tonic,'inh_dopamod_cpg')
			
		#Connect inhibitory populations to mnps
		dopa.create_dopa_connections(inh1.inh_pop,mnp2.motor_neuron_pop,'inh_dopamod_mn')
		dopa.create_dopa_connections(inh2.inh_pop,mnp1.motor_neuron_pop,'inh_dopamod_mn')
	'''
	#Connect excitatory rg neurons
	conn.create_connections(rg1.rg_exc_pop,rg2.rg_exc_pop,'exc_weak')
	
	conn.create_connections(rg2.rg_exc_pop,rg1.rg_exc_pop,'exc_weak')



print("Seed#: ",nn.rng_seed)
print("CPG: # RG: ",nn.rg_pop_neurons,"; # inh buffer: ",nn.inh_pop_neurons)
print("MN: # exc inter: ",nn.num_exc_inter_pop,"; # mnp: ",nn.num_motor_neurons)

init_time=1
t_start = time.perf_counter()
nest.Simulate(nn.sim_time)
num_steps = int(nn.sim_time/nn.time_resolution)
nest.print_time = True
###############################################################################################################
"""
senders_exc_inter_pop1,spiketimes_exc_inter_pop1 = popfunc.read_spike_data(exc1.spike_detector_exc_inter_tonic)
senders_exc_inter_pop2,spiketimes_exc_inter_pop2 = popfunc.read_spike_data(exc2.spike_detector_exc_inter_tonic)

print(spiketimes_exc_inter_pop1)
sg_dopa_tonic1 = nest.Create("spike_generator", params={"spike_times": spiketimes_exc_inter_pop1})  
sg_dopa_tonic2 = nest.Create("spike_generator", params={"spike_times": spiketimes_exc_inter_pop2}) 
        #self.sg_dopa_phasic = nest.Create("inhomogeneous_poisson_generator", params={"rate_times": self.time_dopa_rates,"rate_values":self.value_dopa_rates})

#Create parrot neuron to send dopaminergic spikes
vt_parrot1 = nest.Create("parrot_neuron")
vt_parrot2 = nest.Create("parrot_neuron")
#Connect dopaminergic spike generators to the parrot neuron
nest.Connect(sg_dopa_tonic1, vt_parrot1)
nest.Connect(sg_dopa_tonic2, vt_parrot2)
#nest.Connect(self.sg_dopa_phasic, self.vt_parrot)

#connect parrot neurons to MNP
nest.Connect(vt_parrot1, mnp1.motor_neuron_pop,syn_spec={"synapse_model": "static_synapse",
                                  "weight": 1.0,
                                  "delay": 1.} )  

nest.Connect(vt_parrot2, mnp2.motor_neuron_pop,syn_spec={"synapse_model": "static_synapse",
                                  "weight": 1.0,
                                  "delay": 1.} ) 
"""
###########################################################################################

t_stop = time.perf_counter()    
print('Simulation completed. It took ',round(t_stop-t_start,2),' seconds.')

spike_count_array = []
#Read spike data - rg populations
senders_exc1,spiketimes_exc1 = popfunc.read_spike_data(rg1.spike_detector_rg_exc)

senders_exc2,spiketimes_exc2 = popfunc.read_spike_data(rg2.spike_detector_rg_exc)


#Read spike data - ecitatory interneurons populations

senders_exc_inter_pop1,spiketimes_exc_inter_pop1 = popfunc.read_spike_data(exc1.spike_detector_exc_inter_tonic)
#senders_exc_inter_bursting1,spiketimes_exc_inter_bursting1 = popfunc.read_spike_data(exc1.spike_detector_exc_inter_bursting)
senders_exc_inter_pop2,spiketimes_exc_inter_pop2 = popfunc.read_spike_data(exc2.spike_detector_exc_inter_tonic)
#senders_exc_inter_bursting2,spiketimes_exc_inter_bursting2 = popfunc.read_spike_data(exc2.spike_detector_exc_inter_bursting)
#Read spike data - motor neuron populations
senders_mnp1,spiketimes_mnp1 = popfunc.read_spike_data(mnp1.spike_detector_motor)
senders_mnp2,spiketimes_mnp2 = popfunc.read_spike_data(mnp2.spike_detector_motor)

#Read spike data - inhibitory populations
if nn.rgs_connected==1:
	senders_inhpop1,spiketimes_inhpop1 = popfunc.read_spike_data(inh1.spike_detector_inh)
	senders_inhpop2,spiketimes_inhpop2 = popfunc.read_spike_data(inh2.spike_detector_inh)


#Create Rate Coded Output
if nn.rate_coded_plot==1:
	t_start = time.perf_counter()
	spike_bins_rg_exc1,bin = popfunc.rate_code_spikes_nofilter(nn.rg_pop_neurons,spiketimes_exc1)

	spike_bins_rg1 = spike_bins_rg_exc1
	#spike_bins_rg1 = (spike_bins_rg1-np.min(spike_bins_rg1))/(np.max(spike_bins_rg1)-np.min(spike_bins_rg1))

	spike_bins_rg_exc2,bin = popfunc.rate_code_spikes_nofilter(nn.rg_pop_neurons,spiketimes_exc2)

	spike_bins_rg2 = spike_bins_rg_exc2
	#spike_bins_rg2 = (spike_bins_rg2-np.min(spike_bins_rg2))/(np.max(spike_bins_rg2)-np.min(spike_bins_rg2))
	spike_bins_rgs = spike_bins_rg1+spike_bins_rg2
	
	spike_bins_exc_inter_tonic1,bin = popfunc.rate_code_spikes_nofilter(nn.num_exc_inter_tonic,spiketimes_exc_inter_pop1)
	spike_bins_exc_inter1 = spike_bins_exc_inter_tonic1
	#spike_bins_exc_inter1 = (spike_bins_exc_inter1-np.min(spike_bins_exc_inter1))/(np.max(spike_bins_exc_inter1)-np.min(spike_bins_exc_inter1))
	spike_bins_exc_inter_tonic2,bin = popfunc.rate_code_spikes_nofilter(nn.num_exc_inter_tonic,spiketimes_exc_inter_pop2)
	spike_bins_exc_inter2 = spike_bins_exc_inter_tonic2
	#spike_bins_exc_inter2 = (spike_bins_exc_inter2-np.min(spike_bins_exc_inter2))/(np.max(spike_bins_exc_inter2)-np.min(spike_bins_exc_inter2))
   
	spike_bins_mnp1,bin = popfunc.rate_code_spikes_nofilter(nn.num_motor_neurons,spiketimes_mnp1)
	#spike_bins_mnp1 = (spike_bins_mnp1-np.min(spike_bins_mnp1))/(np.max(spike_bins_mnp1)-np.min(spike_bins_mnp1))
	spike_bins_mnp2,bin = popfunc.rate_code_spikes_nofilter(nn.num_motor_neurons,spiketimes_mnp2)
	#spike_bins_mnp2 = (spike_bins_mnp2-np.min(spike_bins_mnp2))/(np.max(spike_bins_mnp2)-np.min(spike_bins_mnp2))
	spike_bins_mnps = spike_bins_mnp1+spike_bins_mnp2
	
	if nn.rgs_connected==1:
		spike_bins_inh1,bin = popfunc.rate_code_spikes_nofilter(nn.inh_pop_neurons,spiketimes_inhpop1)
		#spike_bins_inh1 = (spike_bins_inh1-np.min(spike_bins_inh1))/(np.max(spike_bins_inh1)-np.min(spike_bins_inh1))
		spike_bins_inh2,bin = popfunc.rate_code_spikes_nofilter(nn.inh_pop_neurons,spiketimes_inhpop2)
		#spike_bins_inh2 = (spike_bins_inh2-np.min(spike_bins_inh2))/(np.max(spike_bins_inh2)-np.min(spike_bins_inh2))
		spike_bins_inh = spike_bins_inh1+spike_bins_inh2
		spike_bins_all_pops = spike_bins_rgs+spike_bins_inh

	t_stop = time.perf_counter()
	print('Rate coded activity complete, taking ',int(t_stop-t_start),' seconds.')
#Create eng 
# eng_mn1=popfunc.extract_rat_eng(nn.num_motor_neurons,spiketimes_mnp1,samplingRate = 1000.,delay_ms=2)
# eng_mn2=popfunc.extract_rat_eng(nn.num_motor_neurons,spiketimes_mnp2,samplingRate = 1000.,delay_ms=2)

# print(len(eng_mn1))



#Create Rate Coded Output2 
smoothed_spike_bins_rg1 = popfunc.rate_code_spikes(nn.rg_pop_neurons,spiketimes_exc1)
smoothed_spike_bins_rg2 = popfunc.rate_code_spikes(nn.rg_pop_neurons,spiketimes_exc2)



smoothed_spike_bins_inh1 =popfunc.rate_code_spikes(nn.inh_pop_neurons,spiketimes_inhpop1)
smoothed_spike_bins_inh2 =popfunc.rate_code_spikes(nn.inh_pop_neurons,spiketimes_inhpop2)



smoothed_spike_bins_exc_inter1 = popfunc.rate_code_spikes(nn.num_exc_inter_tonic,spiketimes_exc_inter_pop1)
smoothed_spike_bins_exc_inter2 = popfunc.rate_code_spikes(nn.num_exc_inter_tonic,spiketimes_exc_inter_pop2)

smoothed_spike_bins_mnp1 = popfunc.rate_code_spikes(nn.num_motor_neurons,spiketimes_mnp1)
smoothed_spike_bins_mnp2 = popfunc.rate_code_spikes(nn.num_motor_neurons,spiketimes_mnp2)

t_rg = np.arange(0,len(smoothed_spike_bins_rg1),1)
t_exc = np.arange(0,len(smoothed_spike_bins_rg1),1)
t_mn = np.arange(0,len(smoothed_spike_bins_mnp1),1)
t_inh=np.arange(0,len(smoothed_spike_bins_inh1),1)
	#bins=np.arange(0, nn.sim_time+nn.time_resolution,nn.time_resolution)
fig, ax = plt.subplots(4,tight_layout=True,sharex='all')
plt.subplots_adjust(wspace=0.3, hspace=0.3)

ax[0].plot(t_rg, smoothed_spike_bins_rg1)
ax[0].plot(t_rg, smoothed_spike_bins_rg2)
ax[1].plot(t_inh, smoothed_spike_bins_inh1)
ax[1].plot(t_inh, smoothed_spike_bins_inh2)
ax[2].plot(t_exc, smoothed_spike_bins_exc_inter1)
ax[2].plot(t_exc, smoothed_spike_bins_exc_inter2) 
ax[3].plot(t_mn, smoothed_spike_bins_mnp1,label='Flexor')
ax[3].plot(t_mn, smoothed_spike_bins_mnp2,label='Extensor')
	
for i in range(4):
	ax[i].set_ylabel('Force (%)')
	ax[i].legend(['Flexor','Extensor'],loc='upper right',prop={'size': 9})
ax[3].set_xlabel('Time (ms)')
custom_xticks=[5000,10000,20000,30000]
custom_xticklabels=['.5','1','2','3']
ax[3].set_xticks(custom_xticks)
ax[3].set_xticklabels(custom_xticklabels)

	#ax[0].set_ylabel("RG activation")
	#ax[1].set_ylabel("Inh activation")
	#ax[2].set_ylabel("Exc activation")
	#ax[3].set_ylabel("MNP activation")
ax[0].set_title("RG")
ax[1].set_title("Inhibitory Interneurons")
ax[2].set_title("Excitatory Interneurons")
ax[3].set_title("Motor Neurons")
	
figure = plt.gcf() # get current figure
figure.set_size_inches(15, 12)
plt.tight_layout()
if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'rate_coded_output_new.png',bbox_inches="tight")

# membrane potential of a neuron in RG population 
'''
vm1,t_vm1,Inacth_NaP1,I_syn_inh1,I_syn_exc1=popfunc.read_membrane_potentiall(mnp1.mm_motor,nn.num_motor_neurons,60)
vm2,t_vm2,Inacth_NaP2,I_syn_inh2,I_syn_exc2=popfunc.read_membrane_potentiall(mnp2.mm_motor,nn.num_motor_neurons,60)
'''
#v_m1_mn,t_m1_mn,Inacth_NaP1_mn1 = popfunc.read_membrane_potential(mnp1.mm_motor,nn.num_motor_neurons)
#v_m2_mn,t_m2_mn,Inacth_NaP2_mn2= popfunc.read_membrane_potential(mnp2.mm_motor,nn.num_motor_neurons)


	


#Plot rate-coded output+ Membrane potential
if nn.rate_coded_plot==1:
	t = np.arange(0,len(spike_bins_rg1),1)
	#bins=np.arange(0, nn.sim_time+nn.time_resolution,nn.time_resolution)
	fig, ax = plt.subplots(4,tight_layout=True,sharex='all')
	plt.subplots_adjust(wspace=0.3, hspace=0.3)
	ax[0].plot(t, spike_bins_rg1, label="I_stim = %.1f pA")
	ax[0].plot(t, spike_bins_rg2)
	ax[1].plot(t, spike_bins_inh1)
	ax[1].plot(t, spike_bins_inh2)		
	ax[2].plot(t, spike_bins_exc_inter1)
	ax[2].plot(t, spike_bins_exc_inter2) 
	ax[3].plot(t, spike_bins_mnp1,label='Flexor')
	ax[3].plot(t, spike_bins_mnp2,label='Extensor')
	# ax[4].plot(t_vm1, vm1, label=r"V_m")
	# ax[4].plot(t_vm2, vm2, label=r"V_m")
	# ax[4].set_ylim(0, np.round(1.1*np.amax(vm1)/50)*50)
	# ax[5].plot(t_vm1, Inacth_NaP1, label=r"Inacth_NaP1_mn1")
	# ax[5].plot(t_vm1, Inacth_NaP1, label=r"Inacth_NaP2_mn2")
	# ax[6].plot(t, eng_mn1, label=r"eng_mn1")
	# ax[6].plot(t, eng_mn1, label=r"eng_mn1")
	#ax[5].set_ylim(0, np.round(1.1*np.amax(Inacth_NaP1)/50)*50)

	for i in range(3):
		ax[i].set_xticks([])
		ax[i].set_xlim(0,nn.sim_time)
		ax[i].set_ylabel('Force (%)')
		ax[i].set_xticks(np.arange(35,65, 10))
	ax[3].set_xlabel('Time (ms)')
	
	ax[3].set_xticks(np.arange(0,nn.sim_time, 100))
	#ax[0].set_ylabel("RG activation")
	#ax[1].set_ylabel("Inh activation")
	#ax[2].set_ylabel("Exc activation")
	#ax[3].set_ylabel("MNP activation")
	ax[0].set_title("RG")
	ax[1].set_title("Inhibitory Interneurons")
	ax[2].set_title("Excitatory Interneurons")
	ax[3].set_title("Motor Neurons")
   
	figure = plt.gcf() # get current figure
	figure.set_size_inches(15, 12)
	plt.tight_layout()
	if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'rate_coded_output.png',bbox_inches="tight")


#Plot raster plot of individual spikes
if nn.raster_plot==0:
	pylab.figure()
	pylab.subplot(211)
	for i in range(nn.rg_pop_neurons-1): 
		pylab.plot(spiketimes_exc1[0][i],senders_exc1[0][i],'.',label='Exc')
  
	pylab.xlabel('Time (ms)')
	pylab.ylabel('Neuron #')
	pylab.title('Spike Output RGs')
	pylab.subplot(212)
	for i in range(nn.rg_pop_neurons-1): 
		pylab.plot(spiketimes_exc2[0][i],senders_exc2[0][i],'.',label='Exc')

	pylab.xlabel('Time (ms)')
	pylab.ylabel('Neuron #')
	#pylab.title('Spike Output rg2')
	pylab.subplots_adjust(bottom=0.15)
	if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'spikes_rg.png',bbox_inches="tight")

	if nn.rgs_connected==1:
		pylab.figure()
		pylab.subplot(211)
		for i in range(nn.inh_pop_neurons-1): 
			pylab.plot(spiketimes_inhpop1[0][i],senders_inhpop1[0][i],'.',label='Inh Pop1')
		pylab.xlabel('Time (ms)')
		pylab.ylabel('Neuron #')
		pylab.title('Spike Output Inh')
		pylab.subplot(212)
		for i in range(nn.inh_pop_neurons-1): 
			pylab.plot(spiketimes_inhpop2[0][i],senders_inhpop2[0][i],'.',label='Inh Pop2')
		pylab.xlabel('Time (ms)')
		pylab.ylabel('Neuron #')
		#pylab.title('Spike Output Inh2')
		pylab.subplots_adjust(bottom=0.15)
		if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'spikes_inh.png',bbox_inches="tight")
	

		pylab.figure()
		pylab.subplot(211)
		for i in range(nn.num_exc_inter_pop-1): 
			pylab.plot(spiketimes_exc_inter_pop1[0][i],senders_exc_inter_pop1[0][i],'.',label='Inh Pop1')
		pylab.xlabel('Time (ms)')
		pylab.ylabel('Neuron #')
		pylab.title('Spike Output Exc Inter neuron')
		pylab.subplot(212)
		for i in range(nn.num_exc_inter_pop-1): 
			pylab.plot(spiketimes_exc_inter_pop2[0][i],senders_exc_inter_pop2[0][i],'.',label='Inh Pop2')
		pylab.xlabel('Time (ms)')
		pylab.ylabel('Neuron #')
		#pylab.title('Spike Output Inh2')
		pylab.subplots_adjust(bottom=0.15)
		if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'spikes_Exc_Int.png',bbox_inches="tight")


		pylab.figure()
		pylab.subplot(211)
		for i in range(nn.num_motor_neurons-1): 
			pylab.plot(spiketimes_mnp1[0][i],senders_mnp1[0][i],'.',label='Inh Pop1')
		pylab.xlabel('Time (ms)')
		pylab.ylabel('Neuron #')
		pylab.title('Spike Output Motor neuron')
		pylab.subplot(212)
		for i in range(nn.num_exc_inter_pop-1): 
			pylab.plot(spiketimes_mnp2[0][i],senders_mnp2[0][i],'.',label='Inh Pop2')
		pylab.xlabel('Time (ms)')
		pylab.ylabel('Neuron #')
		#pylab.title('Spike Output Inh2')
		pylab.subplots_adjust(bottom=0.15)

		if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'spikes_MN.png',bbox_inches="tight")
'''
if nn.neuron_V_NaP_I_syn==0:
	pylab.figure()
	pylab.subplot(211)
	fig, ax = plt.subplots(4,tight_layout=True,sharex='all')
	plt.subplots_adjust(wspace=0.3, hspace=0.3)
	ax[0].plot(t_vm1, vm1, label=r"V_m")
	ax[0].plot(t_vm2, vm2, label=r"V_m")
#ax[0].set_ylim(0, np.round(1.1*np.amax(vm1)/50)*50)
#ax[1].set_ylim(0, np.round(1.1*np.amax(vm2)/50)*50)
	ax[1].plot(t_vm1, Inacth_NaP1, label=r"Inacth_NaP1_mn1")
	ax[1].plot(t_vm1, Inacth_NaP2, label=r"Inacth_NaP2_mn2")
	ax[2].plot(t_vm1, I_syn_inh1, label=r"I_syn_inh1")
	ax[2].plot(t_vm1, I_syn_inh2, label=r"I_syn_inh2")
	ax[3].plot(t_vm1, I_syn_exc1, label=r"I_syn_exc1")
	ax[3].plot(t_vm1, I_syn_exc2, label=r"I_syn_exc2")
#ax[2].set_ylim(0, np.round(1.1*np.amax(Inacth_NaP1)/50)*50)
#ax[3].set_ylim(0, np.round(1.1*np.amax(Inacth_NaP2)/50)*50)
	figure = plt.gcf() # get current figure
	figure.set_size_inches(15, 12)
	plt.tight_layout()
	if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'V_m_MN.png',bbox_inches="tight")
'''
'''
pylab.figure()
pylab.subplot(211)
pylab.plot(t_m1_rg,v_m1_rg)
pylab.title('Individual Neuron Membrane Potential')
pylab.subplot(212)
pylab.plot(t_m2_rg,v_m2_rg)
pylab.xlabel('Time (ms)')
pylab.ylabel('Membrane potential (mV)')
if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'membrane_potential_bursting.png',bbox_inches="tight")
'''




# min_length = min(len(arr) for arr in spiketimes_mnp1)

# # Pad each array to make them of equal length
# padded_arrays = [np.pad(arr, (0, len(arr)-min_length), 'constant') for arr in spiketimes_mnp1]

# # Convert the list of padded arrays to a 2D NumPy array
# two_d_array = np.vstack(padded_arrays)

# # Insert a column of np.array before each array
# column_to_insert = np.array(['np.array'] * len(spiketimes_mnp1)).reshape(-1, 1)
# two_d_array_with_column = np.insert(two_d_array, 0, column_to_insert, axis=1)

# print(two_d_array_with_column)
# save_path='/home/atiye/hh_cpg/simple model/MNP_spikes.npy'
# np.save(save_path,nest.GetStatus(mnp1.spike_detector_motor)[0])
spike_matrix_mn1=popfunc.ENG_structure(nn.num_motor_neurons,mnp1.spike_detector_motor)
spike_matrix_mn2=popfunc.ENG_structure(nn.num_motor_neurons,mnp2.spike_detector_motor)
#print(spike_matrix_mn1)
if nn.args['save_results']:
	# Save rate-coded output
	np.savetxt(nn.pathFigures + '/times_matrix_mn1.csv',spike_matrix_mn1,delimiter=',')
	np.savetxt(nn.pathFigures + '/times_matrix_mn2.csv',spike_matrix_mn2,delimiter=',')

	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_mnp1.csv',spike_bins_mnp1,delimiter=',')
	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_mnp2.csv',spike_bins_mnp2,delimiter=',')
	
	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_inh1.csv',spike_bins_mnp1,delimiter=',')
	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_inh2.csv',spike_bins_mnp2,delimiter=',')
	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_rg1.csv',spike_bins_mnp1,delimiter=',')
	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_rg2.csv',spike_bins_mnp2,delimiter=',')
	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_exc_inter1.csv',spike_bins_mnp1,delimiter=',')
	np.savetxt(nn.pathFigures + '/smoothed_spike_bins_exc_inter2.csv',spike_bins_mnp2,delimiter=',')



#print(spike_matrix_mn)
# coef, freqs=popfunc.ENG(spike_matrix_mn1)
# Fs=1000
# tclip = 500e-3
# nos = np.int64(Fs*tclip)  #No of samples in 10ms
# tpoints = np.linspace(0, 500e-3, nos) #Time points
# print(coef)
# plt.figure(figsize=(15, 10))
# plt.imshow(abs(coef), extent=[0, 10e-3, 20, 1], interpolation='bilinear', cmap='copper',
# 		   aspect='auto', vmax=abs(coef).max(), vmin=abs(coef).max())
# #plt.gca().invert_yaxis()
# plt.yticks(np.arange(1, 21, 1))
# plt.xticks(np.arange(0, nos/Fs, nos/(20*Fs)))
# plt.show()

# # Plotting
# plt.figure(figsize=(15, 10))
# plt.plot(tpoints, spike_matrix_mn1)
# plt.grid(color='gray', linestyle=':', linewidth=0.5)
# plt.show()

# eng_mn=popfunc.extract_rat_eng(spike_matrix_mn1)
# print(eng_mn)
# plt.plot(eng_mn, label = 'eng')
# plt.show()






			



# spikes_rg=nest.GetStatus(rg1.spike_detector_rg_exc)[0]["events"]["senders"]
# print(spikes_rg)
# spikes_rg=nest.GetStatus(rg2.spike_detector_rg_exc)[0]["events"]["senders"]
# print(spikes_rg)
# spikes_exc=nest.GetStatus(exc1.spike_detector_exc_inter_tonic)[0]["events"]["senders"]
# print(spikes_exc)
# spikes_exc=nest.GetStatus(exc2.spike_detector_exc_inter_tonic)[0]["events"]["senders"]
# print(spikes_exc)
# spikes_exc=nest.GetStatus(inh1.spike_detector_inh)[0]["events"]["senders"]
# print(spikes_exc)
# spikes_exc=nest.GetStatus(inh2.spike_detector_inh)[0]["events"]["senders"]
# print(spikes_exc)
# spikes_mnp=nest.GetStatus(mnp1.spike_detector_motor)[0]["events"]["senders"]
# print(spikes_mnp)
# spikes_exc=nest.GetStatus(mnp2.spike_detector_motor)[0]["events"]["senders"]
# print(spikes_exc)

# save_path='/home/atiye/hh_cpg/simple model/exc_inter_spikes.npy'
# np.save(save_path,nest.GetStatus(exc1.spike_detector_exc_inter_tonic )[0])
# save_path='/home/atiye/hh_cpg/simple model/ing_inter_spikes.npy'
#np.save(save_path,nest.GetStatus(inh1.spike_detector_inh)[0])


		
#Estimate frequency (RGs)	
rg1_peaks = find_peaks(spike_bins_rg1,height=0.4,distance=1000)[0]
rg2_peaks = find_peaks(spike_bins_rg2,height=0.4,distance=1000)[0]
avg_rg1_peaks = np.mean(np.diff(rg1_peaks))
avg_rg2_peaks = np.mean(np.diff(rg2_peaks))
avg_rg_peaks = (avg_rg1_peaks+avg_rg2_peaks)/2
print("Peaks RG1: ",rg1_peaks," Average diff (RG1): ",round(avg_rg1_peaks,2))
print("Peaks RG2: ",rg2_peaks," Average diff (RG2): ",round(avg_rg2_peaks,2))
print("Average diff (RG1+2), Freq: ",round(avg_rg_peaks,2),round(1000/(avg_rg_peaks*nn.time_resolution),2))

#Estimate frequency (MNPs)
mnp1_peaks = find_peaks(spike_bins_mnp1,height=0.4,distance=1000)[0]
mnp2_peaks = find_peaks(spike_bins_mnp2,height=0.4,distance=1000)[0]
avg_mnp1_peaks = np.mean(np.diff(mnp1_peaks))
avg_mnp2_peaks = np.mean(np.diff(mnp2_peaks))
avg_mnp_peaks = (avg_mnp1_peaks+avg_mnp2_peaks)/2
print("Peaks MNP1: ",mnp1_peaks," Average diff (MNP1): ",round(avg_mnp1_peaks,2))
print("Peaks MNP2: ",mnp2_peaks," Average diff (MNP2): ",round(avg_mnp2_peaks,2))
print("Average diff (MNP1+2), Freq: ",round(avg_mnp_peaks,2),round(1000/(avg_mnp_peaks*nn.time_resolution),2))


# Generate excitation signals file for OpenSim using normalized rate-coded output
if nn.excitation_file_generation_arm== 1 or nn.excitation_file_generation_leg == 1:
    file_gen(t,spike_bins_mnp2,spike_bins_mnp1,nn.path,nn.excitation_file_generation_arm,nn.excitation_file_generation_leg,nn.excitation_gain)