#include <neuron.hpp>
#include <network.hpp>
#include <layer.hpp>
#include <edge.hpp>
#include <libSimAnneal/libAnnealMD.hpp>
#include <random>
#include <cmath>
#include <iostream>
#include <chrono>
#include <vector>

// network parameters
constexpr static unsigned num_inputs = 2;
constexpr static unsigned num_outputs = 1;
constexpr static unsigned num_hidden = 1;
constexpr static unsigned num_layers = num_hidden + 2;
// training parameters
constexpr static const char* rule_str = "XOR";
auto rule = [](bool* arg_bits) -> bool {
    return arg_bits[0] ^ arg_bits[1];
};
constexpr static unsigned sample_input = 5;
constexpr static unsigned training_data_sz = 4;
constexpr static unsigned p_dist_mean = 1000;
constexpr static const char* dist_name = "Poisson";
// annealing parameters
constexpr static double T0 = 0.01;
constexpr static double Tf = 0.00001;
constexpr static double tau = 3000000.0;
static std::vector<double> wgts_init = {1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, -2.0, 1.0};
static std::vector<double> bias_init = {-0.5, -0.5, -0.5, -1.5, -0.5, -0.5};
static std::vector<double> wgts_bias_init(15, 0);
constexpr static double scale = 0.2;
// test parameters
const int test_data_sz = 4;

int main_other() {
    using namespace LibNeuron;
    using namespace SimAnneal;
    int layer_sz_arr[num_layers] = {num_inputs, 3, num_outputs};
    std::vector<double> spl_wgts = {1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, -2.0, 1.0};
    std::vector<double> spl_bias = {-0.5, -0.5, -0.5, -1.5, -0.5, -0.5};
    Network spl_network(layer_sz_arr, num_layers);
    spl_network.set_weights(spl_wgts);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < layer_sz_arr[i]; j++) {
            for (int k = 0; k < layer_sz_arr[i + 1]; k++) {
                //std::cout << spl_network.get_weights()[i][j][k] << std::endl;
            }
        } 
    }
    spl_network.set_biases(spl_bias);
    constexpr int bits_arr_sz = 4;
    bool** bits_arr = new bool*[bits_arr_sz];
    for (int i = 0; i < bits_arr_sz; i++) {
        bits_arr[i] = new bool[2];
    }
    bits_arr[0][0] = 0;
    bits_arr[0][1] = 0;
    bits_arr[1][0] = 0;
    bits_arr[1][1] = 1;
    bits_arr[2][0] = 1;
    bits_arr[2][1] = 0;
    bits_arr[3][0] = 1;
    bits_arr[3][1] = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < layer_sz_arr[i]; j++) {
            //std::cout << spl_network.get_biases()[i][j] << std::endl;
        }
    }
    for (int i = 0; i < bits_arr_sz; i++) {
        std::cout << spl_network(bits_arr[i])[0] << std::endl;
        //spl_network(bits_arr[i])[0];
    }
}

int main() {
    using namespace LibNeuron;
    using namespace SimAnneal;
    int layer_sz_arr[num_layers] = {num_inputs, 3, num_outputs};
    Network test_network(layer_sz_arr, num_layers);
    //test_network.set_biases(bias_init);
    constexpr int bits_arr_sz = 4;
    bool** bits_arr = new bool*[bits_arr_sz];
    for (int i = 0; i < bits_arr_sz; i++) {
        bits_arr[i] = new bool[2];
    }
    bits_arr[0][0] = 0;
    bits_arr[0][1] = 0;
    bits_arr[1][0] = 0;
    bits_arr[1][1] = 1;
    bits_arr[2][0] = 1;
    bits_arr[2][1] = 0;
    bits_arr[3][0] = 1;
    bits_arr[3][1] = 1;


    std::function<double(std::vector<double>)> energy_fn = [&](std::vector<double> wgts_bias) -> double{
        std::vector<double> wgts(wgts_bias.begin(), wgts_bias.begin() + wgts_init.size());
        std::vector<double> bias(wgts_bias.begin() + wgts_init.size(), wgts_bias.end());
        test_network.set_weights(wgts);
        test_network.set_biases(bias);
        double diff_sum = 0.0;
        for (int i = 0; i < bits_arr_sz; i++) {
            double* network_out = test_network(bits_arr[i]);
            diff_sum += fabs(double(network_out[0]) - double(rule(bits_arr[i])));
            delete[] network_out;
        }
        return diff_sum;
    };
    AnnealMD m_dim_anneal(energy_fn, T0, Tf, tau, wgts_bias_init, scale);
    std::vector<double> result = m_dim_anneal();
    for (int i = 0; i < wgts_init.size(); i++) {
        std::cout << result[i] << " " << std::endl;
    }
    //m_dim_anneal.show_plot();
    std::cout << energy_fn(result) << std::endl;
    for (int i = 0; i < bits_arr_sz; i++) {
        double* network_out = test_network(bits_arr[i]);
        std::cout << network_out[0] << " " << rule(bits_arr[i]) << std::endl;
        delete[] network_out;
    }
    for (int i = 0; i < bits_arr_sz; i++) {
        delete[] bits_arr[i];
    }
    delete[] bits_arr;
}
