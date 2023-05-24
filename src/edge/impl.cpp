#include "impl.hpp"
Impl::Impl() : tip(nullptr), weight(0.0) {}
Impl::Impl(Neuron* tip, float w) : tip(tip), weight(w) {}
float Impl::get_weight() {
    return this->weight;
}
void Impl::set_weight(float w) {
    this->weight = w;
}
void Impl::set_tip(const Neuron& arg_neuron) {
    this->tip = const_cast<Neuron*>(&arg_neuron);
}
void Impl::transmit(float signal) {
    this->tip->signal_add(weight*signal);
}
Impl& Impl::operator=(const Impl& arg_impl) {
    if (this == &arg_impl) {
        return *this;
    } 
    this->tip = arg_impl.tip;
    this->weight = arg_impl.weight;
    return *this;
}
Impl::~Impl() {}
