#ifndef NEURON_IMPL_HPP
#define NEURON_IMPL_HPP
#include <neuron/neuron.hpp>
#include <edge/edge.hpp>
#include <network/network.hpp>
#include <algorithm>
#include <cmath>
#include <random>
#include <assert.h>
#include <functional>
#include <iostream>
#include <chrono>
#include <utility>

using namespace LibNeuron;
class NeuronImpl {
protected:
    Edge* output_edges;
    unsigned sz;
    double input_signal;
    double bias;
public:
    NeuronImpl();
    NeuronImpl(Neuron* arg_neuron_arr, unsigned arg_sz);
    // getters
    Edge* get_edges() const; 
    unsigned get_size() const; 
    double get_input_signal() const;
    double* get_weights() const;
    double get_bias() const;
    double activation() const;
    // setters
    void set_edges(Neuron* arg_neuron_arr, unsigned arg_sz);
    void set_edges(Edge* arg_edge_arr, unsigned arg_sz);
    void signal_add(double arg_signal);
    void set_weights(double* arg_wgts);
    void fire();
    void reset_signal();
    void set_bias(double arg_bias);
    NeuronImpl& operator=(const NeuronImpl& arg_impl);
    ~NeuronImpl();
};
#endif
