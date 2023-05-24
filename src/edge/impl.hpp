#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP
#include <edge.hpp>
#include <neuron.hpp>
using namespace LibNeuron;
class Impl {
    Neuron* tip;
    float weight;
public:
    Impl();
    Impl(Neuron* tip, float w);
    float get_weight();
    void set_weight(float w);
    void set_tip(const Neuron& arg_neuron);
    void transmit(float signal);
    Impl& operator=(const Impl& arg_impl);
    ~Impl();
};
#endif
