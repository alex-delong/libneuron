#include <edge.hpp>
#include <neuron.hpp>
#include <layer.hpp>
#include <network.hpp>
#include <assert.h>
using namespace LibNeuron;
class NetworkImpl {
public:
    Layer* layer_arr;
    unsigned sz;
    NetworkImpl();
    NetworkImpl(const Network& arg_network); 
    NetworkImpl(int* layer_sz_arr, int network_sz);
    unsigned get_size() const;  
    const Layer& get_layer(unsigned i) const; 
    double*** get_weights() const;
    double** get_biases() const;
    void set_weights(double*** arg_weights);
    void set_weights(std::vector<double> arg_wgts);
    void set_biases(std::vector<double> arg_biases);
    // TODO: void add_layer(const Layer& arg_layer) {}
    // TODO: void add_layers(int* layer_sz_arr, int num_layers) {}

    // for a given binary value as input, return the final processed output binary value
    // pre-conditions:
    //      argument is a binary array that represents the input value
    // post-conditions:
    //      returns a binary array from the output neurons
    double* operator()(bool* bin_arr_in) const;
    ~NetworkImpl();
};

