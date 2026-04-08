


import population_functions as popfunc
import pylab
import create_mnp as mnp
import set_network_params as netparams
import create_exc_inter_pop as exc
import matplotlib.pyplot as plt
import nest

nest.Install("nestml_hh_module")
nn=netparams.neural_network()
mnp1 = mnp.create_mnp()
mnp2 = mnp.create_mnp()
exc1 = exc.create_exc_inter_population()
exc2 = exc.create_exc_inter_population()

senders_exc_inter_pop1,spiketimes_exc_inter_pop1 = popfunc.read_spike_data(exc1.spike_detector_exc_inter_tonic)
senders_exc_inter_pop2,spiketimes_exc_inter_pop2 = popfunc.read_spike_data(exc2.spike_detector_exc_inter_tonic)

senders_mnp1,spiketimes_mnp1 = popfunc.read_spike_data(mnp1.spike_detector_motor)
senders_mnp2,spiketimes_mnp2 = popfunc.read_spike_data(mnp2.spike_detector_motor)

indiv_spikes_mnp1,neuron_to_sample_mnp1,sparse_count13,silent_count13 = popfunc.count_indiv_spikes(nn.num_motor_neurons,senders_mnp1)
indiv_spikes_mnp2,neuron_to_sample_mnp2,sparse_count14,silent_count14 = popfunc.count_indiv_spikes(nn.num_motor_neurons,senders_mnp2)

indiv_spikes_exc_inter_tonic1,neuron_to_sample_exc_inter_tonic1,sparse_count9,silent_count9 = popfunc.count_indiv_spikes(nn.num_exc_inter_tonic,senders_exc_inter_pop1)
indiv_spikes_exc_inter_tonic2,neuron_to_sample_exc_inter_tonic2,sparse_count11,silent_count11 = popfunc.count_indiv_spikes(nn.num_exc_inter_tonic,senders_exc_inter_pop2)


# read membrane potential of a hh motor neuron 
v_m1_mn,t_m1_mn = popfunc.read_membrane_potential(mnp1.mm_motor,nn.num_motor_neurons,neuron_to_sample_mnp1)
v_m2_mn,t_m2_mn = popfunc.read_membrane_potential(mnp2.mm_motor,nn.num_motor_neurons,neuron_to_sample_mnp2)

#Read membrane potential of a hh excitatory interneuron 
v_m1_exc_int,t_m1_exc_int = popfunc.read_membrane_potential(exc1.mm_rg_exc_tonic,nn.exc_tonic_count,neuron_to_sample_exc_inter_tonic1)
v_m2_exc_int,t_m2_exc_int = popfunc.read_membrane_potential(exc2.mm_rg_exc_tonic,nn.exc_tonic_count,neuron_to_sample_exc_inter_tonic2)
	
pylab.figure()
pylab.subplot(211)
pylab.plot(t_m1_mn,v_m1_mn)
pylab.title('Individual Neuron Membrane Potential')
pylab.subplot(212)
pylab.plot(t_m2_mn,v_m2_mn)
pylab.xlabel('Time (ms)')
pylab.ylabel('Membrane potential (mV)')
if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'membrane_potential_bursting.png',bbox_inches="tight")

pylab.figure()
pylab.subplot(211)
pylab.plot(t_m1_exc_int,v_m1_exc_int)
pylab.title('Individual Neuron Membrane Potential')
pylab.subplot(212)
pylab.plot(t_m2_exc_int,v_m2_exc_int)
pylab.xlabel('Time (ms)')
pylab.ylabel('Membrane potential (mV)')
if nn.args['save_results']: plt.savefig(nn.pathFigures + '/' + 'membrane_potential_tonic.png',bbox_inches="tight")