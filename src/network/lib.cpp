#include <network.hpp>
#include <neuron.hpp>
#include <layer.hpp>
#include <edge.hpp>
#include <binfuns.hpp>
#include <libAnnealMD.hpp>
#include <assert.h>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include <functional>
#include <iostream>
#include "impl.hpp"
using namespace LibNeuron;
class Network::Impl : public NetworkImpl {
public:
    Impl() : NetworkImpl() {}
    Impl(const Network& arg_network) : NetworkImpl(arg_network) {}
    Impl(int* layer_sz_arr, int network_sz) : NetworkImpl(layer_sz_arr, network_sz) {}
};

unsigned Network::get_size() const {
    return this->pimpl->get_size();
}
const Layer& Network::get_layer(unsigned i) const {
    return this->pimpl->get_layer(i);
}
double*** Network::get_weights() const {
    return this->pimpl->get_weights();
}
double** Network::get_biases() const {
    return this->pimpl->get_biases();
}
void Network::set_weights(double*** arg_wgts) {
    this->pimpl->set_weights(arg_wgts);
}
void Network::set_weights(std::vector<double> arg_wgts) {
    this->pimpl->set_weights(arg_wgts);
}
void Network::set_biases(std::vector<double> arg_biases) {
    this->pimpl->set_biases(arg_biases);
}
Network::Network() : pimpl(new Impl) {}
Network::Network(const Network& arg_network) : pimpl(new Impl(arg_network)) {}
/*unsigned int Network::operator()(unsigned input) const {
    bool* bin_arr_in = BinFuns::to_bin(input);
    bool* bin_arr_out = (*this->pimpl)(bin_arr_in); 
    unsigned unsigned_out = BinFuns::to_int(bin_arr_out);
    return unsigned_out;
}*/
double* Network::operator()(bool* arg_bits) const {
    return (*this->pimpl)(arg_bits);
}
Network::Network(int* layer_sz_arr, int network_sz) : pimpl(new Impl(layer_sz_arr, network_sz)) {}
Network::~Network() {
    delete pimpl;
}
