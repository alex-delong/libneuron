#ifndef LIBLAYER_HPP
#define LIBLAYER_HPP
#include <random>

namespace LibNeuron {
    class Neuron; 
    class Network;
    class Layer {
        class Impl;
        Impl* pimpl;
    public:
        Layer();
        Layer(unsigned int);
        unsigned get_size() const;
        const Neuron& get_neuron(unsigned) const;
        Neuron* get_arr() const;        
        // connects this layer to the argument layer
        // this is done for each neuron in this layer according to Neuron::set_edges
        void connect(const Layer&);        
        void fire(); 
        // add the input argument to the input signal of every neuron in this layer
        void signal_add(float); 
        // return the sum of the input signals in this layer
        float signal_sum(); 
        // resets the input signals for all neurons in the array
        void reset_signal();
        Layer& operator=(const Layer&); 
        // read input signals and convert each to a 1 if positive, else 0
        // then return a binary array from the converted values
        bool* to_bin();
        float* get_output();
        void bin_init(bool*);
        ~Layer();
    };
}
#endif
