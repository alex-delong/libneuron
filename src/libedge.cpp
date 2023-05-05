#include <libneuron.hpp>
#include <libedge.hpp>
using namespace LibNeuron;
class Edge::Impl {
    Neuron* tip;
    float weight;
public:
    Impl() : tip(nullptr), weight(0.0) {}
    Impl(Neuron* tip, float w) : tip(tip), weight(w) {}
    float get_weight() {
        return this->weight;
    }
    void set_weight(float w) {
        this->weight = w;
    }
    void set_tip(const Neuron& arg_neuron) {
        this->tip = const_cast<Neuron*>(&arg_neuron);
    }
    void transmit(float signal) {
        this->tip->signal_add(weight*signal);
    }
    Impl& operator=(const Impl& arg_impl) {
        if (this == &arg_impl) {
            return *this;
        } 
        this->tip = arg_impl.tip;
        this->weight = arg_impl.weight;
        return *this;
    }
    ~Impl() {}
};

Edge::Edge() : pimpl(new Impl) {}
Edge::Edge(Neuron* tip, float w) : pimpl(new Impl(tip, w)) {}
float Edge::get_weight() {
    return this->pimpl->get_weight();
}
void Edge::set_weight(float w) {
    this->pimpl->set_weight(w);
}
void Edge::set_tip(const Neuron& arg_neuron) {
    this->pimpl->set_tip(arg_neuron);
}
void Edge::transmit(float signal) {
    this->pimpl->transmit(signal);
}
Edge& Edge::operator=(const Edge& arg_edge) {
    if (this == &arg_edge) {
        return *this;
    }
    *this->pimpl = *arg_edge.pimpl;
    return *this;
}
Edge::~Edge() {
    delete this->pimpl;
}
