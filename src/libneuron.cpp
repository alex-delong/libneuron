#include <libneuron.hpp>
#include <algorithm>
#include <cmath>
using namespace LibNeuron;
Edge::Edge() : tip(nullptr), weight(1.0) {}
Edge::Edge(Neuron* tip, float w) : tip(tip), weight(w) {}
void Edge::set_weight(float w) {
    this->weight = w;
}
void Edge::set_tip(const Neuron& arg_neuron) {
    this->tip = const_cast<Neuron*>(&arg_neuron);
}
void Edge::transmit(float signal) {
    this->tip->signal_add(weight*signal);
}
Edge::~Edge() {
    delete this->tip;
}

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
    for (int i = 0; i < sz; i++) {
        output_edges[i].set_tip(arg_neuron_arr[i]);
    }    
}
void Neuron::signal_add(float arg_signal) {
    this->input_signal += arg_signal;
}

float Neuron::activation() {
    auto sigmoid = [](float x) -> float {
        return 1/(1 + exp(-x));
    };
    return sigmoid(this->input_signal);
}

void Neuron::fire() {
    for (int i = 0; i < this->sz; i++) {
        this->output_edges[i].transmit(this->activation());
    }
}
