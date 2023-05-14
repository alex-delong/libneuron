#include <liblayer.hpp>
#include <libneuron.hpp>
#include <libnetwork.hpp>
#include <functional>
#include <random>
#include <chrono>
#include <iostream>
using namespace LibNeuron;

class Layer::Impl {
    Neuron* neuron_arr;
    // number of neurons
    unsigned int sz;
public:
    Impl() : neuron_arr(nullptr), sz(0) {}
    Impl(unsigned int arg_sz) : neuron_arr(new Neuron[arg_sz]), sz(arg_sz) {}
    unsigned int get_size() const {
        return this->sz;
    }
    const Neuron& get_neuron(unsigned i) const {
        return this->neuron_arr[i];
    }
    Neuron* get_arr() const {
        return this->neuron_arr;
    }
    // connects this layer to the argument layer
    // this is done for each neuron in this layer according to Neuron::set_edges
    void connect(const Layer& arg_layer) {
        for (int i = 0; i < this->sz; i++) {
            this->neuron_arr[i].set_edges(arg_layer.pimpl->neuron_arr, arg_layer.pimpl->sz);
        }
    }
    void fire() {
        for (int i = 0; i < this->sz; i++) {
            this->neuron_arr[i].fire();
        }
    }
    // add the input argument to the input signal of every neuron in this layer
    void signal_add(float input) {
        std::function<void(int)> add_fn;
        add_fn = [&](int i) {
            if (i < this->sz) {
                this->neuron_arr[i].signal_add(input);
                add_fn(i + 1);
            }
        };
        add_fn(0);
    }
    // return the sum of the input signals in this layer
    float signal_sum() {
        std::function<float(int)> sum_fn;
        sum_fn = [&](int i) -> float {
            if (i < this->sz) {
                return this->neuron_arr[i].get_input_signal() + sum_fn(i + 1);
            } else {
                return 0.0;
            }
        };
        return sum_fn(0);
    }
    // resets the input signals for all neurons in the array
    void reset_signal() {
        std::function<void(int)> reset_fn;
        reset_fn = [&](int i) {
            if (i < this->sz) {
                this->neuron_arr[i].reset_signal();
                reset_fn(i + 1);
            }
        };
        reset_fn(0);
    }
    Impl& operator=(const Impl& arg_impl) {
        if (this == &arg_impl) {
            return *this;
        }
        this->sz = arg_impl.sz;
        delete[] this->neuron_arr;
        this->neuron_arr = new Neuron[this->sz];
        for (int i = 0; i < this->sz; i++) {
            this->neuron_arr[i] = arg_impl.neuron_arr[i];
        }
        return *this;
    }
    bool* to_bin() {
        bool* bin_arr = new bool[this->sz];
        for (int i = 0; i < this->sz; i++) {
            if (this->neuron_arr[i].get_input_signal() > 0) {
                bin_arr[i] = 1;
            } else {
                bin_arr[i] = 0;
            }
        }
        return bin_arr;
    }

    float* get_output() {
        float* out_arr = new float[this->sz];
        for (int i = 0; i < this->sz; i++) {
            out_arr[i] = this->neuron_arr[i].activation();
            //out_arr[i] = this->neuron_arr[i].get_input_signal() + this->neuron_arr[i].get_bias();
        }
        return out_arr;
    }

    void bin_init(bool* arg_bin_arr) {
        for (int i = 0; i < this->sz; i++) {
            this->neuron_arr[i].reset_signal();
            this->neuron_arr[i].signal_add(float(arg_bin_arr[i]));
        }
    }
    ~Impl() {
        delete[] this->neuron_arr;
    }
};

Layer::Layer() : pimpl(new Impl) {}
Layer::Layer(unsigned sz) : pimpl(new Impl(sz)) {}
unsigned Layer::get_size() const {
    return this->pimpl->get_size();
}
const Neuron& Layer::get_neuron(unsigned i) const {
    return this->pimpl->get_neuron(i);
}
Neuron* Layer::get_arr() const {
    return this->pimpl->get_arr();
}
// connects this layer to the argument layer
// this is done for each neuron in this layer according to Neuron::set_edges
void Layer::connect(const Layer& arg_layer) {
    this->pimpl->connect(arg_layer);
}
void Layer::fire() {
    this->pimpl->fire();
} 
// add the input argument to the input signal of every neuron in this layer
void Layer::signal_add(float arg_signal) {
    this->pimpl->signal_add(arg_signal);
} 
// return the sum of the input signals in this layer
float Layer::signal_sum() {
    return this->pimpl->signal_sum();
} 
// resets the input signals for all neurons in the array
void Layer::reset_signal() {
    this->pimpl->reset_signal();
}
Layer& Layer::operator=(const Layer& arg_layer) {
    if (this == &arg_layer) {
        return *this;
    }
    *this->pimpl = *arg_layer.pimpl;
    return *this;
} 
// read input signals and convert each to a 1 if positive, else 0
// then return a binary array from the converted values
bool* Layer::to_bin() {
    return this->pimpl->to_bin();
}
float* Layer::get_output() {
    return this->pimpl->get_output();
}
void Layer::bin_init(bool* arg_bin_arr) {
    this->pimpl->bin_init(arg_bin_arr);
}
Layer::~Layer() {
    delete this->pimpl;
}
