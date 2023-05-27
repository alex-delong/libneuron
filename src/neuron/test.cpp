#include "test.hpp"
#include <neurontest.hpp>
using namespace LibNeuron;
NeuronTest::Impl::Impl() : NeuronImpl() {}
NeuronTest::Impl::Impl(Neuron* arg_neuron_arr, unsigned arg_sz) : NeuronImpl(arg_neuron_arr, arg_sz) {}
const NeuronTest::Impl& NeuronTest::Impl::test_get_edges() const {
    if (this->output_edges != this->get_edges()) {
        throw std::logic_error("get_edges() did not return the correct value");
    }
    return *this;
}
const NeuronTest::Impl& NeuronTest::Impl::test_get_size() const {
    if (this->sz != this->get_size()) {
        throw std::logic_error("get_size() did not return the correct value");
    }
    return *this;
}
const NeuronTest::Impl& NeuronTest::Impl::test_get_weights() const {
    double* test_out = this->get_weights();
    for (unsigned i = 0; i < this->sz; i++) {
        if (test_out[i] != this->output_edges[i].get_weight()) {
            throw std::logic_error("get_weights() did not return the correct value");
        }
    }
    delete[] test_out;
    return *this;
}
const NeuronTest::Impl& NeuronTest::Impl::test_get_bias() const {
    if (this->bias != this->get_bias()) {
        throw std::logic_error("get_bias() did not return the correct value");
    }
    return *this;
}
const NeuronTest::Impl& NeuronTest::Impl::test_get_input_signal() const {
    if (this->input_signal != this->get_input_signal()) {
        throw std::logic_error("get_input_signal() did not return the correct value");
    }
    return *this;
}

NeuronTest::NeuronTest() : pimpl(new Impl) {}
NeuronTest::NeuronTest(Neuron* arg_neuron_arr, unsigned arg_sz) : pimpl(new Impl(arg_neuron_arr, arg_sz)) {}
const NeuronTest& NeuronTest::test_get_edges() const {
    this->pimpl->test_get_edges();
    return *this;
}
const NeuronTest& NeuronTest::test_get_size() const {
    this->pimpl->test_get_size();
    return *this;
}
const NeuronTest& NeuronTest::test_get_weights() const {
    this->pimpl->test_get_size();
    return *this;
}
const NeuronTest& NeuronTest::test_get_bias() const {
    this->pimpl->test_get_bias();
    return *this;
}
const NeuronTest& NeuronTest::test_get_input_signal() const {
    this->pimpl->test_get_input_signal();
    return *this;
}
NeuronTest::~NeuronTest() {
    delete this->pimpl;
}
