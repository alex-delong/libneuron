#include <libnetwork.hpp>
#include <libneuron.hpp>
#include <liblayer.hpp>
#include <libedge.hpp>
#include <libbinfuns.hpp>
#include <libSimAnneal/libAnnealMD.hpp>
#include <assert.h>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include <functional>
#include <iostream>
using namespace LibNeuron;
class Network::Impl {
public:
    Layer* layer_arr;
    unsigned sz;
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
    unsigned get_size() const {
        return this->sz;
    }
    const Layer& get_layer(unsigned i) const {
        return this->layer_arr[i];
    }
    double*** get_weights() const {
        double*** out_arr = new double**[this->sz];
        for (int i = 0; i < this->sz; i++) {
            out_arr[i] = new double*[this->layer_arr[i].get_size()];
            for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
                out_arr[i][j] = this->layer_arr[i].get_arr()[j].get_weights(); 
            }
        }
        return out_arr;
    }
    double** get_biases() const {
        double** out_arr = new double*[this->sz];
        for (int i = 0; i < this->sz; i++) {
            out_arr[i] = new double[this->layer_arr[i].get_size()];
            for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
                out_arr[i][j] = this->layer_arr[i].get_arr()[j].get_bias();
            }
        }
        return out_arr;
    }
    void set_weights(double*** arg_weights) {
        for (int i = 0; i < this->sz; i++) {
            for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
                this->layer_arr[i].get_arr()[j].set_weights(arg_weights[i][j]);
            }
        }
        delete[] arg_weights;
    }
    void set_weights(std::vector<double> arg_wgts) {
        int curr = 0;
        for (int i = 0; i < this->sz; i++) {
            int j_max = this->layer_arr[i].get_size();
            for (int j = 0; j < j_max; j++) {
                int k_max = this->layer_arr[i].get_arr()[j].get_size();
                for (int k = 0; k < k_max; k++) {
                    this->layer_arr[i].get_arr()[j].get_edges()[k].set_weight(arg_wgts[curr]);
                    curr += 1;
                }
            }
        }
    }
    void set_biases(std::vector<double> arg_biases) {
        int curr = 0;
        for (int i = 0; i < this->sz; i++) {
            int j_max = this->layer_arr[i].get_size();
            for (int j = 0; j < j_max; j++) {
                this->layer_arr[i].get_arr()[j].set_bias(arg_biases[curr]);
                curr += 1;
            }
        }
    }
    // TODO: void add_layer(const Layer& arg_layer) {}
    // TODO: void add_layers(int* layer_sz_arr, int num_layers) {}

    // for a given binary value as input, return the final processed output binary value
    // pre-conditions:
    //      argument is a binary array that represents the input value
    // post-conditions:
    //      returns a binary array from the output neurons
    double* operator()(bool* bin_arr_in) const {
        assert(this->layer_arr != nullptr);
        this->layer_arr[0].bin_init(bin_arr_in);
        for (int i = 0; i < this->sz - 1; i++) {
            this->layer_arr[i].fire();
        } 
        // get output from last layer
        double* output = this->layer_arr[sz - 1].get_output();
        this->layer_arr[sz - 1].reset_signal();
        //delete[] bin_arr_in;
        return output;
    }
    ~Impl() {
        delete[] this->layer_arr;
    }
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
