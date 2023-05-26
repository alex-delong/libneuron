#ifndef NEURON_TEST_HPP
#define NEURON_TEST_HPP
namespace LibNeuron {
    class Neuron;
    class TestImpl {
        class Impl;
        Impl* pimpl;
    public:
        TestImpl(); 
        TestImpl(Neuron* arg_neuron_arr, unsigned arg_sz); 
        void test_get_edges();
        void test_get_size();
        void test_get_weights();
        void test_get_bias();
        void test_get_input_signal();
    };
}
#endif
