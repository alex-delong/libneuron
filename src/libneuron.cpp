#include <libneuron.hpp>
#include <algorithm>
#include <cmath>
#include <random>
#include <assert.h>
#include <functional>
#include <iostream>
#include <chrono>
#include <utility>
using namespace LibNeuron;
bool is_odd(int i) {
    return i % 2 != 0;
}
void init_bin(bool* bin_arr) {
    for (int j = 0; j < 32; j++) {
        bin_arr[j] = 0;
    }
}
//  post-conditions: 
//      returns an array of bools that represent the argument in binary  
bool* to_bin(unsigned int i) {
    unsigned int curr_pwr = 0;
    bool* bin_arr = new bool[32];
    init_bin(bin_arr);
    unsigned int result = i;
    while (result != 0 || curr_pwr > 31) {
        if (is_odd(result)) {
            bin_arr[31 - curr_pwr] = 1;
            result -= 1;
        } else {
            curr_pwr += 1;
            result = result / 2;
        }
    }
    return bin_arr;
}
unsigned int to_int(bool* bin_arr) {
    unsigned int output = 0;
    for (int i = 0; i < 32; i++) {
        output += pow(2, i)*bin_arr[31 - i];
    }
    return output;
}

Edge::Edge() : tip(nullptr), weight(0.0) {}
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
Edge& Edge::operator=(const Edge& arg_edge) {
    if (this == &arg_edge) {
        return *this;
    } 
    this->tip = arg_edge.tip;
    this->weight = arg_edge.weight;
    return *this;
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
void Neuron::metropolis(const Network& arg_network, unsigned int arg_input_signal, unsigned int expectation, float T) { 
    // get error for current weights
    unsigned int curr_err = abs(arg_network(arg_input_signal) - expectation);
    // generate a normal distribution to sample from
    std::default_random_engine generator(std::time(nullptr));
    std::normal_distribution<float> norm_distribution(0.0, 1.0); // mean, stddev
    // probability function for whether to accept or reject candidate edges
    auto P = [](int delta_e, float T) -> float {
        return exp(-delta_e/T);
    }; 
    // store the current edges in case the candidate edges are rejected
    Edge* old_edges = this->output_edges;
    // generate candidate edges whose weights are the current weights + a random increment 
    Edge* new_edges = new Edge[this->sz];
    for (int i = 0; i < this->sz; i++) {
        float delta_w = norm_distribution(generator);
        new_edges[i] = this->output_edges[i];
        float curr_wgt = new_edges[i].get_weight();
        new_edges[i].set_weight(curr_wgt + delta_w);
    }
    // find the error for the new edges
    this->output_edges = new_edges;
    unsigned int candidate_err = abs(arg_network(arg_input_signal) - expectation);
    int delta_e = candidate_err - curr_err;
    // determine if the new edges are accepted using probability P.
    std::uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
    float threshold = uniform_distribution(generator);
    if (P(delta_e, T) > threshold) {
        // accept new_edges
        delete[] old_edges;
    } else {
        this->output_edges = old_edges;
        delete[] new_edges;
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
            for (int i = 0; i < this->sz; i++) {
                this->neuron_arr[i].set_edges(arg_layer.neuron_arr, arg_layer.sz);
            }
        }
        void fire() {
            for (int i = 0; i < this->sz; i++) {
                this->neuron_arr[i].fire();
            }
        }
        // add the input argument to the input signal of every neuron in this layer
        void signal_add(float input) {
            std::function<void(int)> add_fn;
            add_fn = [&](int i) {
                if (i < this->sz) {
                    this->neuron_arr[i].signal_add(input);
                    add_fn(i + 1);
                }
            };
            add_fn(0);
        }
        // return the sum of the input signals in this layer
        float signal_sum() {
            std::function<float(int)> sum_fn;
            sum_fn = [&](int i) -> float {
                if (i < this->sz) {
                    return this->neuron_arr[i].read_signal() + sum_fn(i + 1);
                } else {
                    return 0.0;
                }
            };
            return sum_fn(0);
        }
        // resets the input signals for all neurons in the array
        void reset_signal() {
            std::function<void(int)> reset_fn;
            reset_fn = [&](int i) {
                if (i < this->sz) {
                    this->neuron_arr[i].reset_signal();
                    reset_fn(i + 1);
                }
            };
            reset_fn(0);
        }
        void metropolis(const Network& arg_network, float arg_input_signal, float expectation, float T) {
            for (int i = 0; i < this->sz; i++) {
                this->neuron_arr[i].metropolis(arg_network, arg_input_signal, expectation, T);
            }
        }
        Layer& operator=(const Layer& arg_layer) {
            if (this == &arg_layer) {
                return *this;
            }
            this->sz = arg_layer.sz;
            delete[] this->neuron_arr;
            this->neuron_arr = new Neuron[this->sz];
            for (int i = 0; i < this->sz; i++) {
                this->neuron_arr[i] = arg_layer.neuron_arr[i];
            }
            return *this;
        }
        // read input signals and convert each to a 1 if positive, else 0
        // then return a binary array from the converted values
        bool* to_bin() {
            bool* out = new bool[this->sz];
            for (int i = 0; i < this->sz; i++) {
                if (this->neuron_arr[i].read_signal() > 0) {
                    out[i] = 1;
                } else {
                    out[i] = 0;
                }
            }
            return out;
        }
        void bin_init(bool* arg_bin_arr) {
            this->reset_signal();
            for (int i = 0; i < this->sz; i++) {
                this->neuron_arr[i].signal_add(arg_bin_arr[i]); 
            }
        }
        ~Layer() {
            delete[] this->neuron_arr;
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
        for (int i = 0; i < this->sz; i++) {
            layer_arr[i] = Layer(layer_sz_arr[i]);
        }
        for (int i = 0; i < this->sz - 1; i++) {
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
    // for a given binary value as input, return the final processed output binary value
    // pre-conditions:
    //      argument is a binary array that represents the input value
    // post-conditions:
    //      returns a binary array from the output neurons
    bool* operator()(bool* bin_arr_in) const {
        assert(this->layer_arr != nullptr);
        this->layer_arr[0].bin_init(bin_arr_in);
        for (int i = 0; i < this->sz - 1; i++) {
            this->layer_arr[i].fire();
        } 
        // get output from last layer
        bool* output = this->layer_arr[sz - 1].to_bin();
        this->layer_arr[sz - 1].reset_signal();
        return output;
    }
    ~Impl() {
        delete[] this->layer_arr;
    }
};
void Network::anneal(float arg_input_signal, float expectation, float T0, float Tf) {
    assert(T0 > Tf);
    float T_inc = (T0 - Tf)/100.0;
    for (float T = T0; T > Tf; T -= T_inc) {
        this->pimpl->metropolis(*this, arg_input_signal, expectation, T);
    }
}
Network::Network() : pimpl(new Impl) {}
Network::Network(const Network& arg_network) : pimpl(new Impl(arg_network)) {}
unsigned int Network::operator()(unsigned int input) const {
    bool* bin_arr_in = to_bin(input);
    bool* bin_arr_out = (*this->pimpl)(bin_arr_in); 
    return to_int(bin_arr_out);
}
Network::Network(int* layer_sz_arr, int network_sz) : pimpl(new Impl(layer_sz_arr, network_sz)) {}
Network::~Network() {
    delete pimpl;
}


