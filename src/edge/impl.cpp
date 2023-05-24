#include "impl.hpp"
EdgeImpl::EdgeImpl() : tip(nullptr), weight(0.0) {}
EdgeImpl::EdgeImpl(Neuron* tip, float w) : tip(tip), weight(w) {}
float EdgeImpl::get_weight() {
    return this->weight;
}
void EdgeImpl::set_weight(float w) {
    this->weight = w;
}
void EdgeImpl::set_tip(const Neuron& arg_neuron) {
    this->tip = const_cast<Neuron*>(&arg_neuron);
}
void EdgeImpl::transmit(float signal) {
    this->tip->signal_add(weight*signal);
}
EdgeImpl& EdgeImpl::operator=(const EdgeImpl& arg_impl) {
    if (this == &arg_impl) {
        return *this;
    } 
    this->tip = arg_impl.tip;
    this->weight = arg_impl.weight;
    return *this;
}
EdgeImpl::~EdgeImpl() {}
