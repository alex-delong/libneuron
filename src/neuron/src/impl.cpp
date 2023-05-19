#include "../include/impl.hpp"

using namespace LibNeuron;


NeuronImpl::NeuronImpl() : output_edges(nullptr), sz(0), input_signal(0.0), bias(0.0) {}
NeuronImpl::NeuronImpl(Neuron* arg_neuron_arr, unsigned arg_sz) :  
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
Edge* NeuronImpl::get_edges() const {
    return this->output_edges;
}
unsigned NeuronImpl::get_size() const {
    return this->sz;
}
double NeuronImpl::get_input_signal() const {
    return this->input_signal;
}
double* NeuronImpl::get_weights() const {
    if (this->output_edges == nullptr) {
        throw std::logic_error("Attempted to get weights from uninitialized edges");
    }
    double* out = new double[this->sz];
    for (unsigned i = 0; i < this->sz; i++) {
        out[i] = this->output_edges[i].get_weight();
    }
    return out;
}
double NeuronImpl::get_bias() const {
    return this->bias;
}
double NeuronImpl::activation() const {
    auto sigmoid = [&](double x) -> double {
        return 1/(1 + exp(-x*10));
    };
    double result = sigmoid(this->input_signal + this->bias);
    return result;
}
// setters
void NeuronImpl::set_edges(Neuron* arg_neuron_arr, unsigned arg_sz) {
    this->sz = arg_sz;
    delete[] this->output_edges;
    this->output_edges = new Edge[arg_sz];
    for (unsigned i = 0; i < arg_sz; i++) {
        this->output_edges[i].set_tip(arg_neuron_arr[i]); 
    }
}
void NeuronImpl::set_edges(Edge* arg_edge_arr, unsigned arg_sz) {
    this->sz = arg_sz;
    delete[] this->output_edges;
    this->output_edges = arg_edge_arr;
}
void NeuronImpl::signal_add(double arg_signal) {
    this->input_signal += arg_signal;
}
void NeuronImpl::set_weights(double* arg_wgts) {
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
void NeuronImpl::fire() {
    assert(this->output_edges != nullptr);
    for (unsigned i = 0; i < this->sz; i++) {
        this->output_edges[i].transmit(this->activation());
    }
    this->input_signal = 0.0;
}
void NeuronImpl::reset_signal() {
    this->input_signal = 0.0;
}
void NeuronImpl::set_bias(double arg_bias) {
    this->bias = arg_bias;
}
NeuronImpl& NeuronImpl::operator=(const NeuronImpl& arg_impl) {
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
NeuronImpl::~NeuronImpl() {
    delete[] this->output_edges;
}


