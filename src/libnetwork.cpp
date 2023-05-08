#include <libnetwork.hpp>
#include <libneuron.hpp>
#include <liblayer.hpp>
#include <libbinfuns.hpp>
#include <assert.h>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>
#include <functional>
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
    float*** get_weights() const {
        float*** out_arr = new float**[this->sz];
        for (int i = 0; i < this->sz; i++) {
            out_arr[i] = new float*[this->layer_arr[i].get_size()];
            for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
                out_arr[i][j] = this->layer_arr[i].get_arr()[j].get_weights(); 
            }
        }
        return out_arr;
    }
    void set_weights(float*** arg_weights) {
        for (int i = 0; i < this->sz; i++) {
            for (int j = 0; j < this->sz; j++) {
                this->layer_arr[i].get_arr()[j].set_weights(arg_weights[i][j]);
            }
        }
        delete[] arg_weights;
    }
    // TODO: void add_layer(const Layer& arg_layer) {}
    // TODO: void add_layers(int* layer_sz_arr, int num_layers) {}
    void metropolis(unsigned arg_input_signal, unsigned expectation, float T) {
        // total number of neurons minus the output layer
        std::function<unsigned(void)> out = [&]() -> unsigned {
            return BinFuns::to_int((*this)(BinFuns::to_bin(arg_input_signal)));
        };
        unsigned curr_err = abs(out() - expectation);
        unsigned N = 0;
        for (int i = 0; i < this->sz - 1; i++) {
            N += this->layer_arr[i].get_size();
        }
        float*** old_weights = this->get_weights();
        // for each layer excluding the output, create a new thread and call metropolis 
        std::thread* thd_arr = new std::thread[N];
        auto thd_fn = [&](unsigned index) {
            this->layer_arr[index].metropolis();
        };
        std::srand(std::time(nullptr));
        for (int i = 0; i < N; i++) {
            unsigned r_index = std::rand() % (this->sz - 1);
            thd_arr[i] = std::thread(thd_fn, r_index);
        }
        for (int i = 0; i < N; i++) {
            thd_arr[i].join();
        }
        delete[] thd_arr;
        unsigned candidate_err = abs(out() - expectation);
        unsigned delta_e = candidate_err - curr_err;
        std::default_random_engine generator(std::time(nullptr));
        std::uniform_real_distribution<float> uni_dist(0.0, 1.0);
        auto P = [&]() -> float {
            return exp(-delta_e/T);
        };
        if (P() < uni_dist(generator)) {
            this->set_weights(old_weights);
        } else {
            for (int i = 0; i < this->sz; i++) {
                for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
                    delete[] old_weights[i][j];
                }
                delete[] old_weights[i];
            }
            delete[] old_weights;
        }
    }
    void metropolis(bool* arg_input_signal, bool expectation, float T) {
        // total number of neurons minus the output layer
        std::function<bool*(void)> out = [&]() -> bool* {
            bool* temp = new bool[this->layer_arr[0].get_size()];
            std::copy(arg_input_signal, arg_input_signal + this->layer_arr[0].get_size(), temp);
            return (*this)(temp);
        };
        bool curr_err = out() - expectation;
        unsigned N = 0;
        for (int i = 0; i < this->sz - 1; i++) {
            N += this->layer_arr[i].get_size();
        }
        float*** old_weights = this->get_weights();
        // for each layer excluding the output, create a new thread and call metropolis 
        std::thread* thd_arr = new std::thread[N];
        auto thd_fn = [&](unsigned index) {
            this->layer_arr[index].metropolis();
        };
        std::srand(std::time(nullptr));
        for (int i = 0; i < N; i++) {
            unsigned r_index = std::rand() % (this->sz - 1);
            thd_arr[i] = std::thread(thd_fn, r_index);
        }
        for (int i = 0; i < N; i++) {
            thd_arr[i].join();
        }
        delete[] thd_arr;
        bool candidate_err = out() - expectation;
        bool delta_e = candidate_err - curr_err;
        std::default_random_engine generator(std::time(nullptr));
        std::uniform_real_distribution<float> uni_dist(0.0, 1.0);
        auto P = [&]() -> float {
            return exp(-delta_e/T);
        };
        if (P() < uni_dist(generator)) {
            this->set_weights(old_weights);
        } else {
            for (int i = 0; i < this->sz; i++) {
                for (int j = 0; j < this->layer_arr[i].get_size(); j++) {
                    delete[] old_weights[i][j];
                }
                delete[] old_weights[i];
            }
            delete[] old_weights;
        }
    }

    void anneal(float arg_input_signal, float expectation, float T0, float Tf) {
        assert(T0 > Tf);
        float T_inc = (T0 - Tf)/1000.0;
        for (float T = T0; T > Tf; T -= T_inc) {
            this->metropolis(arg_input_signal, expectation, T);
        }
    }
    void anneal(bool* arg_input_signal, bool expectation, float T0, float Tf) {
        assert(T0 > Tf);
        float T_inc = (T0 - Tf)/1000.0;
        for (float T = T0; T > Tf; T -= T_inc) {
            this->metropolis(arg_input_signal, expectation, T);
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
        delete[] bin_arr_in;
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
float*** Network::get_weights() const {
    return this->pimpl->get_weights();
}
void Network::anneal(float arg_input_signal, float expectation, float T0, float Tf) {
    this->pimpl->anneal(arg_input_signal, expectation, T0, Tf);
}
void Network::anneal(bool* arg_input_signal, bool expectation, float T0, float Tf) {
    this->pimpl->anneal(arg_input_signal, expectation, T0, Tf);
}

Network::Network() : pimpl(new Impl) {}
Network::Network(const Network& arg_network) : pimpl(new Impl(arg_network)) {}
unsigned int Network::operator()(unsigned input) const {
    bool* bin_arr_in = BinFuns::to_bin(input);
    bool* bin_arr_out = (*this->pimpl)(bin_arr_in); 
    unsigned unsigned_out = BinFuns::to_int(bin_arr_out);
    return unsigned_out;
}
bool* Network::operator()(bool* arg_bits) const {
    return (*this->pimpl)(arg_bits);
}
Network::Network(int* layer_sz_arr, int network_sz) : pimpl(new Impl(layer_sz_arr, network_sz)) {}
Network::~Network() {
    delete pimpl;
}
