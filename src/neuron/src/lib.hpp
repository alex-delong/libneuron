#ifndef NEURON_IMPL_HPP
#define NEURON_IMPL_HPP
#include <libneuron.hpp>
#include <libedge.hpp>
#include <libnetwork.hpp>
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
    NeuronImpl() : output_edges(nullptr), sz(0), input_signal(0.0), bias(0.0) {}
    NeuronImpl(Neuron* arg_neuron_arr, unsigned arg_sz) :  
        output_edges(new Edge[arg_sz]), 
        sz(arg_sz),
        input_signal(0.0),
        bias(0.0)
    {
        for (unsigned i = 0; i < arg_sz; i++) {
            this->output_edges[i].set_tip(arg_neuron_arr[i]);
        }    
    }
    // getters
    Edge* get_edges() const {
        return this->output_edges;
    }
    unsigned get_size() const {
        return this->sz;
    }
    double get_input_signal() const {
        return this->input_signal;
    }
    double* get_weights() const {
        if (this->output_edges == nullptr) {
            throw std::logic_error("Attempted to get weights from uninitialized edges");
        }
        double* out = new double[this->sz];
        for (unsigned i = 0; i < this->sz; i++) {
            out[i] = this->output_edges[i].get_weight();
        }
        return out;
    }
    double get_bias() const {
        return this->bias;
    }
    double activation() const {
        auto sigmoid = [&](double x) -> double {
            return 1/(1 + exp(-x*10));
        };
        double result = sigmoid(this->input_signal + this->bias);
        return result;
    }
    // setters
    void set_edges(Neuron* arg_neuron_arr, unsigned arg_sz) {
        this->sz = arg_sz;
        delete[] this->output_edges;
        this->output_edges = new Edge[arg_sz];
        for (unsigned i = 0; i < arg_sz; i++) {
            this->output_edges[i].set_tip(arg_neuron_arr[i]); 
        }
    }
    void set_edges(Edge* arg_edge_arr, unsigned arg_sz) {
        this->sz = arg_sz;
        delete[] this->output_edges;
        this->output_edges = arg_edge_arr;
    }
    void set_size(unsigned i) {
        this->sz = i;
    }
    void signal_add(double arg_signal) {
        this->input_signal += arg_signal;
    }
    void set_weights(double* arg_wgts) {
        if (this->output_edges == nullptr) {
            throw std::logic_error("Attempted to set weights for uninitialized edges");
        } else if (arg_wgts == nullptr) {
            throw std::invalid_argument("Attempted to set weights from nullptr argument");
        }
        Edge* edges = this->output_edges;
        for (unsigned i = 0; i < this->get_size(); i++) {
            edges[i].set_weight(arg_wgts[i]); 
        }
        delete[] arg_wgts;
    }
    void fire() {
        assert(this->output_edges != nullptr);
        for (unsigned i = 0; i < this->sz; i++) {
            this->output_edges[i].transmit(this->activation());
        }
        this->input_signal = 0.0;
    }
    void reset_signal() {
        this->input_signal = 0.0;
    }
    void set_bias(double arg_bias) {
        this->bias = arg_bias;
    }
    NeuronImpl& operator=(const NeuronImpl& arg_impl) {
        if (this == &arg_impl) {
            return *this;
        }
        delete[] this->output_edges;
        this->sz = arg_impl.sz;
        this->output_edges = new Edge[this->sz];
        for (unsigned i = 0; i < this->sz; i++) {
            this->output_edges[i] = arg_impl.output_edges[i];
        }
        this->input_signal = arg_impl.input_signal;
        return *this;
    }
    ~NeuronImpl() {
        delete[] this->output_edges;
    }
};
#endif
