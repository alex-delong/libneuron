#ifndef LIBNETWORK_HPP
#define LIBNETWORK_HPP
#include <vector>

namespace LibNeuron {
    class Layer;
    class Network {
        class Impl;
        Impl* pimpl;
    public:
        Network();
        Network(const Network&);
        unsigned get_size() const;
        const Layer& get_layer(unsigned) const;
        double*** get_weights() const;
        double** get_biases() const;
        void set_weights(double***);
        void set_weights(std::vector<double>);
        void set_biases(std::vector<double>);
        // initialize an array of sz layers, each with a size given by layer_sz_arr
        Network(int* layer_sz_arr, int sz);
        unsigned operator()(unsigned) const;
        double* operator()(bool*) const;
        ~Network();
    };
}
#endif
