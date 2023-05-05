#include <libnetwork.hpp>
#include <libneuron.hpp>
#include <liblayer.hpp>
#include <libbinfuns.hpp>
#include <assert.h>
#include <random>
#include <chrono>
using namespace LibNeuron;
class Network::Impl {
public:
    Layer* layer_arr;
    unsigned int sz;
    Impl() : layer_arr(nullptr), sz(0) {}
    Impl(const Network& arg_network) : layer_arr(new Layer[arg_network.pimpl->sz]), sz(arg_network.pimpl->sz) {
        for (int i = 0; i < this->sz; i++) {
            this->layer_arr[i] = arg_network.pimpl->layer_arr[i];
        }
    }
    Impl(int* layer_sz_arr, int network_sz) : layer_arr(new Layer[network_sz]), sz(network_sz) {
        for (int i = 0; i < this->sz; i++) {
            layer_arr[i] = Layer(layer_sz_arr[i]);
        }
        for (int i = 0; i < this->sz - 1; i++) {
            layer_arr[i].connect(layer_arr[i + 1]);
        }
    }
    // TODO: void add_layer(const Layer& arg_layer) {}
    // TODO: void add_layers(int* layer_sz_arr, int num_layers) {}
    void metropolis(const Network& arg_network, float arg_input_signal, float expectation, float T) {
        std::srand(std::time(nullptr));
        // call metropolis on a random layer excluding the output layer
        for (int i = 0; i < this->sz - 1; i++) {
            this->layer_arr[std::rand() % this->sz - 1].metropolis(arg_network, arg_input_signal, expectation, T);
        }
    }
    // for a given binary value as input, return the final processed output binary value
    // pre-conditions:
    //      argument is a binary array that represents the input value
    // post-conditions:
    //      returns a binary array from the output neurons
    bool* operator()(bool* bin_arr_in) const {
        assert(this->layer_arr != nullptr);
        this->layer_arr[0].bin_init(bin_arr_in);
        for (int i = 0; i < this->sz - 1; i++) {
            this->layer_arr[i].fire();
        } 
        // get output from last layer
        bool* output = this->layer_arr[sz - 1].to_bin();
        this->layer_arr[sz - 1].reset_signal();
        return output;
    }
    ~Impl() {
        delete[] this->layer_arr;
    }
};
void Network::anneal(float arg_input_signal, float expectation, float T0, float Tf) {
    assert(T0 > Tf);
    float T_inc = (T0 - Tf)/100.0;
    for (float T = T0; T > Tf; T -= T_inc) {
        this->pimpl->metropolis(*this, arg_input_signal, expectation, T);
    }
}
Network::Network() : pimpl(new Impl) {}
Network::Network(const Network& arg_network) : pimpl(new Impl(arg_network)) {}
unsigned int Network::operator()(unsigned input) const {
    bool* bin_arr_in = to_bin(input);
    bool* bin_arr_out = (*this->pimpl)(bin_arr_in); 
    unsigned unsigned_out = to_int(bin_arr_out);
    delete[] bin_arr_in;
    delete[] bin_arr_out;
    return unsigned_out;
}
Network::Network(int* layer_sz_arr, int network_sz) : pimpl(new Impl(layer_sz_arr, network_sz)) {}
Network::~Network() {
    delete pimpl;
}
