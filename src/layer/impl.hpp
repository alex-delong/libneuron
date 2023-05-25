#include <layer.hpp>
#include <neuron.hpp>
#include <edge.hpp>
#include <network.hpp>
using namespace LibNeuron;
struct NetworkImpl {
    Neuron* neuron_arr;
    // number of neurons
    unsigned int sz;
public:
    NetworkImpl();
    NetworkImpl(unsigned int arg_sz); 
    unsigned int get_size() const; 
    const Neuron& get_neuron(unsigned i) const; 
    Neuron* get_arr() const;
    // connects this layer to the argument layer
    // this is done for each neuron in this layer according to Neuron::set_edges
    void connect(const Layer& arg_layer);
    void fire();
    // add the input argument to the input signal of every neuron in this layer
    void signal_add(double input);
    // return the sum of the input signals in this layer
    double signal_sum();
    // resets the input signals for all neurons in the array
    void reset_signal();
    NetworkImpl& operator=(const NetworkImpl& arg_impl);
    bool* to_bin();
    double* get_output();
    void bin_init(bool* arg_bin_arr);
    ~NetworkImpl();
};
