#include <libneuron.hpp>
#include <random>
#include <cmath>
int main() {
    using namespace LibNeuron;
    // make a feed-forward neural network with input size == output size == 1 and 1 hidden layer of size 5
    int layer_sz_arr[3] = {1, 5, 1};
    Network test_network(layer_sz_arr, 3);
    // make training data
    // rule: division by 10
    int training_data_sz = 10;
    float training_input_arr[training_data_sz];
    float training_output_arr[training_data_sz];
    std::default_random_engine generator;
    std::normal_distribution<float> norm_dist;
    for (int i = 0; i < training_data_sz; i++) {
        training_input_arr[i] = norm_dist(generator);
        training_output_arr[i] = training_input_arr[i]/10.0;
    }
    // train the network
    for (int i = 0; i < training_data_sz; i++) {
        test_network.anneal(training_input_arr[i], training_output_arr[i], 10.0, 0.01);
    }
    // generate test data
    int test_data_sz = 10;
    float test_input_arr[test_data_sz];
    float test_output_arr[test_data_sz];
    for (int i = 0; i < test_data_sz; i++) {
        test_input_arr[i] = norm_dist(generator);
        test_output_arr[i] = test_network(test_input_arr[i]);
    }
    // compare to expectation
    float expectation_arr[test_data_sz];
    float abs_error[test_data_sz];
    float pct_error[test_data_sz];
    float mean_abs_error = 0;
    float mean_pct_error = 0;
    for (int i = 0; i < test_data_sz; i++) {
        expectation_arr[i] = test_input_arr[i]/10.0;
        abs_error[i] = fabs(test_output_arr[i] - expectation_arr[i]);
        pct_error[i] = abs_error[i]/expectation_arr[i];
        mean_abs_error += abs_error[i]/test_data_sz;
        mean_pct_error += pct_error[i]/test_data_sz;
    }
}
