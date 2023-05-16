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
#include <thread>
#include <mutex>
#include <stdexcept>
using namespace LibNeuron;
class Neuron::Impl {
    Edge* output_edges;
    unsigned sz;
    double input_signal;
    double bias;
public:
    Impl() : output_edges(nullptr), sz(0), input_signal(0.0), bias(0.0) {}
    Impl(Neuron* arg_neuron_arr, unsigned arg_sz) :  
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
    Impl& operator=(const Impl& arg_impl) {
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
    ~Impl() {
        delete[] this->output_edges;
    }
};
Neuron::Neuron() : pimpl(new Impl) {}
Neuron::Neuron(Neuron* arg_neuron_arr, unsigned int arg_sz) : pimpl(new Impl(arg_neuron_arr, arg_sz)) {}
Edge* Neuron::get_edges() const {
    return this->pimpl->get_edges();
}
unsigned Neuron::get_size() const {
    return this->pimpl->get_size();
}
double* Neuron::get_weights() const {
    return this->pimpl->get_weights();
}
double Neuron::get_bias() const {
    return this->pimpl->get_bias();
}
double Neuron::get_input_signal() const {
    return this->pimpl->get_input_signal();
}
// TODO: void Neuron::set_edges(const Layer&) {} // sets all the edges of this neuron to connect to all the neurons in the argument layer

void Neuron::set_edges(Neuron* arg_neuron_arr, unsigned arg_sz) {
    this->pimpl->set_edges(arg_neuron_arr, arg_sz);
}
void Neuron::set_edges(Edge* arg_edge_arr, unsigned arg_sz) {
    this->pimpl->set_edges(arg_edge_arr, arg_sz);
}
void Neuron::signal_add(double arg_signal) {
    this->pimpl->signal_add(arg_signal);
}
void Neuron::set_weights(double* arg_wgts) {
    this->pimpl->set_weights(arg_wgts);
}
void Neuron::set_bias(double arg_bias) {
    return this->pimpl->set_bias(arg_bias);
}
double Neuron::activation() {
    return this->pimpl->activation();
}
// for every edge in the array of output edges, transmit the processed signal
// then reset the input signal
// pre-conditions:
    // this->output_edges != nullptr
// post-conditions:
    // the input signal has been processed and transmitted to all of the forward neurons
    // this->input_signal == 0;
void Neuron::fire() {
    this->pimpl->fire();
}
void Neuron::reset_signal() {
    this->pimpl->reset_signal();
}
Neuron& Neuron::operator=(const Neuron& arg_neuron) {
    if (this == &arg_neuron) {
        return *this;
    }
    *this->pimpl = *arg_neuron.pimpl;
    return *this;
}
Neuron::~Neuron() {
    delete this->pimpl;
}


