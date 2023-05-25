#include <functional>
#include "impl.hpp"
using namespace LibNeuron;
NetworkImpl::NetworkImpl() : neuron_arr(nullptr), sz(0) {}
NetworkImpl::NetworkImpl(unsigned int arg_sz) : neuron_arr(new Neuron[arg_sz]), sz(arg_sz) {}
unsigned int NetworkImpl::get_size() const {
    return this->sz;
}
const Neuron& NetworkImpl::get_neuron(unsigned i) const {
    return this->neuron_arr[i];
}
Neuron* NetworkImpl::get_arr() const {
    return this->neuron_arr;
}
// connects this layer to the argument layer
// this is done for each neuron in this layer according to Neuron::set_edges
void NetworkImpl::connect(const Layer& arg_layer) {
    for (int i = 0; i < this->sz; i++) {
        this->neuron_arr[i].set_edges(arg_layer.get_arr(), arg_layer.get_size());
    }
}
void NetworkImpl::fire() {
    for (int i = 0; i < this->sz; i++) {
        this->neuron_arr[i].fire();
    }
}
// add the input argument to the input signal of every neuron in this layer
void NetworkImpl::signal_add(double input) {
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
double NetworkImpl::signal_sum() {
    std::function<double(int)> sum_fn;
    sum_fn = [&](int i) -> double {
        if (i < this->sz) {
            return this->neuron_arr[i].get_input_signal() + sum_fn(i + 1);
        } else {
            return 0.0;
        }
    };
    return sum_fn(0);
}
// resets the input signals for all neurons in the array
void NetworkImpl::reset_signal() {
    std::function<void(int)> reset_fn;
    reset_fn = [&](int i) {
        if (i < this->sz) {
            this->neuron_arr[i].reset_signal();
            reset_fn(i + 1);
        }
    };
    reset_fn(0);
}
NetworkImpl& NetworkImpl::operator=(const NetworkImpl& arg_impl) {
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
bool* NetworkImpl::to_bin() {
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

double* NetworkImpl::get_output() {
    double* out_arr = new double[this->sz];
    for (int i = 0; i < this->sz; i++) {
        out_arr[i] = this->neuron_arr[i].activation();
        //out_arr[i] = this->neuron_arr[i].get_input_signal() + this->neuron_arr[i].get_bias();
    }
    return out_arr;
}

void NetworkImpl::bin_init(bool* arg_bin_arr) {
    for (int i = 0; i < this->sz; i++) {
        this->neuron_arr[i].reset_signal();
        this->neuron_arr[i].signal_add(double(arg_bin_arr[i]));
    }
}
NetworkImpl::~NetworkImpl() {
    delete[] this->neuron_arr;
}

