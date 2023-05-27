#ifndef NEURON_TEST_IMPL_HPP
#define NEURON_TEST_IMPL_HPP
#include "impl.hpp"
#include <neurontest.hpp>
using namespace LibNeuron;
class NeuronTest::Impl : public NeuronImpl {   
public:
    Impl(); 
    Impl(Neuron* arg_neuron_arr, unsigned arg_sz); 
    const Impl& test_get_edges() const;
    const Impl& test_get_size() const;
    const Impl& test_get_weights() const;
    const Impl& test_get_bias() const;
    const Impl& test_get_input_signal() const;
};
#endif
