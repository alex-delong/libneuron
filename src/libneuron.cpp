#include <libneuron.hpp>
#include <algorithm>
#include <cmath>
#include <random>
#include <assert.h>
using namespace LibNeuron;
Edge::Edge() : tip(nullptr), weight(1.0) {}
Edge::Edge(Neuron* tip, float w) : tip(tip), weight(w) {}
float Edge::get_weight() {
    return this->weight;
}
Neuron* Edge::get_tip() {
    return this->tip;
}
void Edge::set_weight(float w) {
    this->weight = w;
}
void Edge::set_tip(const Neuron& arg_neuron) {
    this->tip = const_cast<Neuron*>(&arg_neuron);
}
void Edge::transmit(float signal) {
    this->tip->signal_add(weight*signal);
}
Edge::~Edge() {}
Neuron::Neuron() : 
    input_signal(0.0), 
    output_edges(nullptr), 
    sz(0) 
{}
Neuron::Neuron(Neuron* arg_neuron_arr, unsigned int arg_sz) : 
    input_signal(0.0), 
    output_edges(new Edge[arg_sz]), 
    sz(arg_sz) 
{
    for (int i = 0; i < arg_sz; i++) {
        output_edges[i].set_tip(arg_neuron_arr[i]);
    }    
}
void Neuron::set_edges(Neuron* arg_neuron_arr, unsigned int arg_sz) {
    this->sz = arg_sz;
    delete[] this->output_edges;
    this->output_edges = new Edge[arg_sz];
    for (int i = 0; i < arg_sz; i++) {
        this->output_edges[i].set_tip(arg_neuron_arr[i]); 
    }
}
// TODO: void Neuron::set_edges(const Layer&) {}
void Neuron::signal_add(float arg_signal) {
    this->input_signal += arg_signal;
}
float Neuron::activation() {
    auto sigmoid = [](float x) -> float {
        return 1/(1 + exp(-x));
    };
    return sigmoid(this->input_signal);
}
// for every edge in the array of output edges, transmit the processed signal
// then reset the input signal
// pre-conditions:
    // this->output_edges != nullptr
// post-conditions:
    // the input signal has been processed and transmitted to all of the forward neurons
    // this->input_signal == 0;
void Neuron::fire() {
    assert(this->output_edges != nullptr);
    for (int i = 0; i < this->sz; i++) {
        this->output_edges[i].transmit(this->activation());
    }
    this->input_signal = 0;
}
float Neuron::read_signal() {
    return this->input_signal;
}
void Neuron::reset_signal() {
    this->input_signal = 0;
}
Edge* Neuron::get_edges() {
    return this->output_edges;
}
unsigned int Neuron::get_size() {
    return this->sz;
}
// simulated annealing of this neuron using a metropolis algorithm
void Neuron::metropolis(Network arg_network, float arg_input_signal, float expectation, float T) { 
    // get error for current weights
    float curr_err = fabs(arg_network(arg_input_signal) - expectation);
    // generate a normal distribution to sample from
    std::default_random_engine generator;
    std::normal_distribution<float> norm_distribution(0.0, 1.0); // mean, stddev
    // probability function for whether to accept or reject candidate edges
    auto P = [](float delta_e, float T) {
        return exp(-delta_e/T);
    }; 
    // store the current edges in case the candidate edges are rejected
    Edge old_edges[this->sz];
    for (int i = 0; i < this->sz; i++) {
        old_edges[i] = this->output_edges[i];
    }
    // generate candidate edges whose weights are the current weights + a random increment
    float delta_w = norm_distribution(generator);
    Edge new_edges[this->sz];
    for (int i = 0; i < this->sz; i++) {
        new_edges[i] = this->output_edges[i];
        float curr_wgt = new_edges[i].get_weight();
        new_edges[i].set_weight(curr_wgt + delta_w);
    }
    // find the error for the new edges
    this->output_edges = new_edges;
    float candidate_err = fabs(arg_network(arg_input_signal) - expectation);
    float delta_e = candidate_err - curr_err;
    // determine if the new edges are accepted using probability P.
    std::uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
    float threshold = uniform_distribution(generator);
    if (P(delta_e, T) > threshold) {
        // accept new_edges
    } else {
        this->output_edges = old_edges;
    }
}
Neuron& Neuron::operator=(const Neuron& arg_neuron) {
    if (this == &arg_neuron) {
        return *this;
    }
    this->sz = arg_neuron.sz;
    delete[] this->output_edges;
    this->output_edges = new Edge[this->sz];
    for (int i = 0; i < this->sz; i++) {
        this->output_edges[i] = arg_neuron.output_edges[i];
    }
    return *this;
}
Neuron::~Neuron() {
    delete[] this->get_edges();
}
class Network::Impl {
public:
    class Layer {
        Neuron* neuron_arr;
        // number of neurons
        unsigned int sz;
    public:
        Layer() : neuron_arr(nullptr), sz(0) {}
        Layer(unsigned int arg_sz) : neuron_arr(new Neuron[arg_sz]), sz(arg_sz) {}
        unsigned int get_size() const {
            return this->sz;
        }
        Neuron* get_arr() const {
            return this->neuron_arr;
        }
        // connects this layer to the argument layer
        // this is done for each neuron in this layer according to Neuron::set_edges
        void connect(const Layer& arg_layer) {
            for (int i = 0; i < this->get_size(); i++) {
                this->neuron_arr[i].set_edges(arg_layer.get_arr(), arg_layer.get_size());
            }
        }
        void fire() {
            for (int i = 0; i < this->get_size(); i++) {
                this->neuron_arr[i].fire();
            }
        }
        void metropolis(const Network& arg_network, float arg_input_signal, float expectation, float T) {
            for (int i = 0; i < this->get_size(); i++) {
                this->neuron_arr[i].metropolis(arg_network, arg_input_signal, expectation, T);
            }
        }
        Layer& operator=(const Layer& arg_layer) {
            if (this == &arg_layer) {
                return *this;
            }
            this->sz = arg_layer.get_size();
            delete[] this->neuron_arr;
            this->neuron_arr = new Neuron[this->sz];
            for (int i = 0; i < this->sz; i++) {
                this->neuron_arr[i] = arg_layer.get_arr()[i];
            }
            return *this;
        }
        ~Layer() {
            delete[] this->get_arr();
        }
    };
    Layer* layer_arr;
    unsigned int sz;
    Impl() : layer_arr(nullptr), sz(0) {}
    Impl(const Network& arg_network) : layer_arr(new Layer[arg_network.pimpl->sz]), sz(arg_network.pimpl->sz) {
        for (int i = 0; i < this->sz; i++) {
            this->layer_arr[i] = arg_network.pimpl->layer_arr[i];
        }
    }
    Impl(int* layer_sz_arr, int network_sz) : layer_arr(new Layer[network_sz]), sz(network_sz) {
        for (int i = 0; i < sz; i++) {
            layer_arr[i] = Layer(layer_sz_arr[i]);
        }
        for (int i = 0; i < sz - 1; i++) {
            layer_arr[i].connect(layer_arr[i + 1]);
        }
    }
    // TODO: void add_layer(const Layer& arg_layer) {}
    // TODO: void add_layers(int* layer_sz_arr, int num_layers) {}
    void metropolis(const Network& arg_network, float arg_input_signal, float expectation, float T) {
        for (int i = 0; i < this->sz; i++) {
            this->layer_arr[i].metropolis(arg_network, arg_input_signal, expectation, T);
        }
    }
    // for a given argument as input, return the signal recieved by the output neuron
    float operator()(float input) {
        assert(this->layer_arr != nullptr);
        this->layer_arr[0].get_arr()[0].signal_add(input);
        for (int i = 0; i < this->sz - 1; i++) {
            this->layer_arr[i].fire();
        } 
        // get output from last neuron unprocessed
        float output = this->layer_arr[sz - 1].get_arr()[0].read_signal();
        this->layer_arr[sz - 1].get_arr()[0].reset_signal();
        return output;
    }
};
void Network::anneal(float arg_input_signal, float expectation, float T0, float Tf) {
    float T_inc = (Tf - T0)/100.0;
    for (float T = T0; T < Tf; T += T_inc) {
        this->pimpl->metropolis(*this, arg_input_signal, expectation, T);
    }
}
Network::Network() : pimpl(new Impl) {}
Network::Network(const Network& arg_network) : pimpl(new Impl(arg_network)) {}
float Network::operator()(float input) {
    return (*this->pimpl)(input); 
}
Network::Network(int* layer_sz_arr, int network_sz) : pimpl(new Impl(layer_sz_arr, network_sz)) {}
Network::~Network() {
    delete pimpl;
}


