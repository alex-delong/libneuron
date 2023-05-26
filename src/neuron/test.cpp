#include "impl.hpp"
#include <neurontest.hpp>
using namespace LibNeuron;
class TestImpl::Impl : public NeuronImpl {    
    Impl() : NeuronImpl() {}
    Impl(Neuron* arg_neuron_arr, unsigned arg_sz) : NeuronImpl(arg_neuron_arr, arg_sz) {}
    void test_get_edges() {
        if (this->output_edges != this->get_edges()) {
            throw std::logic_error("get_edges() did not return the correct value");
        }
    }
    void test_get_size() {
        if (this->sz != this->get_size()) {
            throw std::logic_error("get_size() did not return the correct value");
        }
    }
    void test_get_weights() {
        double* test_out = this->get_weights();
        for (unsigned i = 0; i < this->sz; i++) {
            if (test_out[i] != this->output_edges[i].get_weight()) {
                throw std::logic_error("get_weights() did not return the correct value");
            }
        }
        delete[] test_out;
    }
    void test_get_bias() {
        if (this->bias != this->get_bias()) {
            throw std::logic_error("get_bias() did not return the correct value");
        }
    }
    void test_get_input_signal() {
        if (this->input_signal != this->get_input_signal()) {
            throw std::logic_error("get_input_signal() did not return the correct value");
        }
    }
};

