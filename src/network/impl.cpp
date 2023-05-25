#include "impl.hpp"
NetworkImpl::NetworkImpl() : layer_arr(nullptr), sz(0) {}
NetworkImpl::NetworkImpl(const Network& arg_network) : layer_arr(new Layer[arg_network.get_size()]), sz(arg_network.get_size()) {
    for (int i = 0; i < this->sz; i++) {
        this->layer_arr[i] = arg_network.get_layer(i);
    }
}
NetworkImpl::NetworkImpl(int* layer_sz_arr, int network_sz) : layer_arr(new Layer[network_sz]), sz(network_sz) {
    for (int i = 0; i < this->sz; i++) {
        layer_arr[i] = Layer(layer_sz_arr[i]);
    }
    for (int i = 0; i < this->sz - 1; i++) {
        layer_arr[i].connect(layer_arr[i + 1]);
    }
}
unsigned NetworkImpl::get_size() const {
    return this->sz;
}
const Layer& NetworkImpl::get_layer(unsigned i) const {
    return this->layer_arr[i];
}
double*** NetworkImpl::get_weights() const {
    double*** out_arr = new double**[this->sz];
    for (int i = 0; i < this->sz; i++) {
        out_arr[i] = new double*[this->layer_arr[i].get_size()];
        for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
            out_arr[i][j] = this->layer_arr[i].get_arr()[j].get_weights(); 
        }
    }
    return out_arr;
}
double** NetworkImpl::get_biases() const {
    double** out_arr = new double*[this->sz];
    for (int i = 0; i < this->sz; i++) {
        out_arr[i] = new double[this->layer_arr[i].get_size()];
        for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
            out_arr[i][j] = this->layer_arr[i].get_arr()[j].get_bias();
        }
    }
    return out_arr;
}
void NetworkImpl::set_weights(double*** arg_weights) {
    for (int i = 0; i < this->sz; i++) {
        for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
            this->layer_arr[i].get_arr()[j].set_weights(arg_weights[i][j]);
        }
    }
    delete[] arg_weights;
}
void NetworkImpl::set_weights(std::vector<double> arg_wgts) {
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
void NetworkImpl::set_biases(std::vector<double> arg_biases) {
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
double* NetworkImpl::operator()(bool* bin_arr_in) const {
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
NetworkImpl::~NetworkImpl() {
    delete[] this->layer_arr;
}
