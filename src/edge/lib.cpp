#include <neuron.hpp>
#include <edge.hpp>
#include "impl.hpp"
using namespace LibNeuron;
class Edge::Impl : public Impl {}

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
