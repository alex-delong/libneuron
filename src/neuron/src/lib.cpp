#include <libneuron.hpp>
#include <libedge.hpp>
#include <libnetwork.hpp>
#include <impl.hpp>

using namespace LibNeuron;
struct Neuron::Impl : public NeuronImpl {
    Impl() : NeuronImpl() {}
    Impl(Neuron* arg_neuron_arr, unsigned arg_sz) : NeuronImpl(arg_neuron_arr, arg_sz) {}
};

Neuron::Neuron() : pimpl(new Impl) {}
Neuron::Neuron(Neuron* arg_neuron_arr, unsigned int arg_sz) : pimpl(new Impl(arg_neuron_arr, arg_sz)) {}
Edge* Neuron::get_edges() const {
    return this->pimpl->get_edges();
}
unsigned Neuron::get_size() const {
    return this->pimpl->get_size();
}
double* Neuron::get_weights() const {
    return this->pimpl->get_weights();
}
double Neuron::get_bias() const {
    return this->pimpl->get_bias();
}
double Neuron::get_input_signal() const {
    return this->pimpl->get_input_signal();
}
// TODO: void Neuron::set_edges(const Layer&) {} // sets all the edges of this neuron to connect to all the neurons in the argument layer

void Neuron::set_edges(Neuron* arg_neuron_arr, unsigned arg_sz) {
    this->pimpl->set_edges(arg_neuron_arr, arg_sz);
}
void Neuron::set_edges(Edge* arg_edge_arr, unsigned arg_sz) {
    this->pimpl->set_edges(arg_edge_arr, arg_sz);
}
void Neuron::signal_add(double arg_signal) {
    this->pimpl->signal_add(arg_signal);
}
void Neuron::set_weights(double* arg_wgts) {
    this->pimpl->set_weights(arg_wgts);
}
void Neuron::set_bias(double arg_bias) {
    return this->pimpl->set_bias(arg_bias);
}
double Neuron::activation() {
    return this->pimpl->activation();
}
// for every edge in the array of output edges, transmit the processed signal
// then reset the input signal
// pre-conditions:
    // this->output_edges != nullptr
// post-conditions:
    // the input signal has been processed and transmitted to all of the forward neurons
    // this->input_signal == 0;
void Neuron::fire() {
    this->pimpl->fire();
}
void Neuron::reset_signal() {
    this->pimpl->reset_signal();
}
Neuron& Neuron::operator=(const Neuron& arg_neuron) {
    if (this == &arg_neuron) {
        return *this;
    }
    *this->pimpl = *arg_neuron.pimpl;
    return *this;
}
Neuron::~Neuron() {
    delete this->pimpl;
}


