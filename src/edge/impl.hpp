#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP
#include <edge.hpp>
#include <neuron.hpp>
using namespace LibNeuron;
class EdgeImpl {
    Neuron* tip;
    float weight;
public:
    EdgeImpl();
    EdgeImpl(Neuron* tip, float w);
    float get_weight();
    void set_weight(float w);
    void set_tip(const Neuron& arg_neuron);
    void transmit(float signal);
    EdgeImpl& operator=(const EdgeImpl& arg_impl);
    ~EdgeImpl();
};
#endif
