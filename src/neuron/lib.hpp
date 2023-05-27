#ifndef NEURON_LIB_HPP
#define NEURON_LIB_HPP
#include <neuron.hpp>
#include "impl.hpp"

using namespace LibNeuron;
struct Neuron::Impl : public NeuronImpl {
    Impl();
    Impl(Neuron* arg_neuron_arr, unsigned arg_sz);
};
#endif
