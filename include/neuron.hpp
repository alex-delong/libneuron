#ifndef LIBNEURON_HPP
#define LIBNEURON_HPP
#include <random>

namespace LibNeuron {
    class Edge;
    class Network;
    class Neuron {
        class Impl;
        Impl* pimpl;
    public:
        // default constructor
        Neuron();
        // constructor to connect to an array of neurons
        Neuron(Neuron* neuron_arr, unsigned neuron_arr_size);
        unsigned get_size() const;
        Edge* get_edges() const;
        double* get_weights() const;
        double get_bias() const;
        // get the unprocessed input signal
        double get_input_signal() const;
        void set_edges(Neuron*, unsigned);
        void set_edges(Edge*, unsigned);
        void set_weights(double*);
        void set_bias(double);
        // add the argument to the input signal
        void signal_add(double);
        // reset the input signal to 0
        void reset_signal();
        // a non-linear function that processes the signal
        double activation();
        // a function to send the processed signal to output edges
        // pre-conditions:
            // all input edges have passed their signals
        // post-conditions:
            // this has transmitted its processed signal to the output edges
            // input_signal == 0
        void fire();
        Neuron& operator=(const Neuron&);
        ~Neuron();
    };
}
#endif
