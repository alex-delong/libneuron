#ifndef NEURON_TEST_HPP
#define NEURON_TEST_HPP
namespace LibNeuron {
    class Neuron;
    class NeuronTest {
        class Impl;
        Impl* pimpl;
    public:
        NeuronTest(); 
        NeuronTest(Neuron* arg_neuron_arr, unsigned arg_sz); 
        const NeuronTest& test_get_edges() const;
        const NeuronTest& test_get_size() const;
        const NeuronTest& test_get_weights() const;
        const NeuronTest& test_get_bias() const;
        const NeuronTest& test_get_input_signal() const;
        ~NeuronTest();
    };
}
#endif
