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
using namespace LibNeuron;
class Neuron::Impl {
    Edge* output_edges;
    unsigned sz;
    float input_signal;
public:
    Impl() : input_signal(0.0), output_edges(nullptr), sz(0) {}
    Impl(Neuron* arg_neuron_arr, unsigned int arg_sz) : 
        input_signal(0.0), 
        output_edges(new Edge[arg_sz]), 
        sz(arg_sz) 
    {
        for (int i = 0; i < arg_sz; i++) {
            this->output_edges[i].set_tip(arg_neuron_arr[i]);
        }    
    }
    Edge* get_edges() {
        return this->output_edges;
    }
    unsigned get_size() {
        return this->sz;
    }
    float get_input_signal() {
        return this->input_signal;
    }
    void set_edges(Neuron* arg_neuron_arr, unsigned arg_sz) {
        this->sz = arg_sz;
        delete[] this->output_edges;
        this->output_edges = new Edge[arg_sz];
        for (int i = 0; i < arg_sz; i++) {
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
    void signal_add(float arg_signal) {
        this->input_signal += arg_signal;
    }
    void reset_signal() {
        this->input_signal = 0.0;
    }
    Impl& operator=(const Impl& arg_impl) {
        if (this == &arg_impl) {
            return *this;
        }
        delete[] this->output_edges;
        this->sz = arg_impl.sz;
        this->output_edges = new Edge[this->sz];
        for (int i = 0; i < this->sz; i++) {
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
unsigned Neuron::get_size() const {
    return this->pimpl->get_size();
}
Edge* Neuron::get_edges() const {
    return this->pimpl->get_edges();
}
float* Neuron::get_weights() const {
    float* out = new float[this->get_size()];
    for (int i = 0; i < this->get_size(); i++) {
        out[i] = this->get_edges()[i].get_weight();
    }
    return out;
}
float Neuron::get_input_signal() const {
    return this->pimpl->get_input_signal();
}
// TODO: void Neuron::set_edges(const Layer&) {} // sets all the edges of this neuron to connect to all the neurons in the argument layer

void Neuron::set_edges(Neuron* arg_neuron_arr, unsigned arg_sz) {
    this->pimpl->set_edges(arg_neuron_arr, arg_sz);
}
void Neuron::set_edges(Edge* arg_edge_arr, unsigned arg_sz) {
    this->pimpl->set_edges(arg_edge_arr, arg_sz);
}
void Neuron::set_weights(float* arg_float_arr) {
    Edge* edges = this->get_edges();
    for (int i = 0; i < this->get_size(); i++) {
        edges[i].set_weight(arg_float_arr[i]); 
    }
    delete[] arg_float_arr;
}
void Neuron::signal_add(float arg_signal) {
    this->pimpl->signal_add(arg_signal);
}
float Neuron::activation() {
    auto sigmoid = [](float x) -> float {
        return 1/(1 + exp(-x));
    };
    return sigmoid(this->get_input_signal());
}
// for every edge in the array of output edges, transmit the processed signal
// then reset the input signal
// pre-conditions:
    // this->output_edges != nullptr
// post-conditions:
    // the input signal has been processed and transmitted to all of the forward neurons
    // this->input_signal == 0;
void Neuron::fire() {
    assert(this->pimpl->get_edges() != nullptr);
    for (int i = 0; i < this->pimpl->get_size(); i++) {
        this->pimpl->get_edges()[i].transmit(this->activation());
    }
    this->reset_signal();
}

void Neuron::reset_signal() {
    this->pimpl->reset_signal();
}
// shift the weights by a random increment
void Neuron::r_shift_weights() {
    std::default_random_engine generator(std::time(nullptr));
    std::normal_distribution<float> norm_distribution(0.0, 1.0); // mean, stddev
    //Edge* new_edges = new Edge[this->get_size()];
    float* new_wgts = this->get_weights();
    for (int i = 0; i < this->get_size(); i++) {
        float delta_w = norm_distribution(generator);
        new_wgts[i] += delta_w;
        //new_edges[i] = this->get_edges()[i];
        //float curr_wgt = new_edges[i].get_weight();
        //new_edges[i].set_weight(curr_wgt + delta_w);
    }
    this->set_weights(new_wgts);
    //this->set_edges(new_edges, this->get_size());
}
// simulated annealing of this neuron using a metropolis algorithm
void Neuron::metropolis(const Network& arg_network, unsigned int arg_input_signal, unsigned int expectation, float T) { 
    // get error for current weights
    unsigned curr_err = abs(arg_network(arg_input_signal) - expectation);
    // generate a normal distribution to sample from
    std::default_random_engine generator(std::time(nullptr));
    std::normal_distribution<float> norm_distribution(0.0, 1.0); // mean, stddev
    // probability function for whether to accept or reject candidate edges
    auto P = [](int delta_e, float T) -> float {
        return exp(-delta_e/T);
    };
    // store the current edges in case the candidate edges are rejected
    Edge* old_edges = new Edge[this->get_size()];
    std::function<void(void)> store = [&]() {
        for (int i = 0; i < this->get_size(); i++) {
            old_edges[i] = this->get_edges()[i];
        }
    };
    store();
    // generate candidate edges whose weights are the current weights + a random increment 
    this->r_shift_weights();
    unsigned candidate_err = abs(arg_network(arg_input_signal) - expectation);
    int delta_e = candidate_err - curr_err;
    // determine if the new edges are accepted using probability P.
    std::uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
    float threshold = uniform_distribution(generator);
    if (P(delta_e, T) > threshold) {
        // accept new_edges
        delete[] old_edges;
    } else {
        this->set_edges(old_edges, this->get_size());
    }
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


