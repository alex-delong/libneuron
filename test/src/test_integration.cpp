#include <libneuron.hpp>
#include <libnetwork.hpp>
#include <random>
#include <cmath>
#include <iostream>
#include <chrono>

// network parameters
const unsigned num_inputs = 32;
const unsigned num_outputs = 32;
const unsigned num_hidden = 1;
const unsigned num_layers = num_hidden + 2;
// training parameters
const char* rule_str = "division by 10";
auto rule = [](unsigned int x) -> unsigned int {
    return x + 5;
};
const unsigned sample_input = 5;
const unsigned training_data_sz = 10;
const unsigned p_dist_mean = 1000;
const char* dist_name = "Poisson";
// annealing parameters
const float T0 = 1.0;
const float Tf = 0.01;
// test parameters
const int test_data_sz = 10;

int main() {
    using std::cout;
    using std::endl;
    using namespace LibNeuron;
    // make network
    cout << "Test network parameters: " << endl
         << "   inputs = " << num_inputs << endl
         << "   outputs = " << num_outputs << endl
         << "   layers = " << num_layers << endl;
    int layer_sz_arr[num_layers] = {num_inputs, 32, num_outputs};
    Network test_network(layer_sz_arr, num_layers);
    // make training data
    cout << "Training parameters: " << endl
         << "   rule = " << rule_str << endl
         << "   training data size = " << training_data_sz << endl;
    unsigned training_input_arr[training_data_sz];
    unsigned training_output_arr[training_data_sz];
    std::default_random_engine generator(std::time(nullptr));
    std::poisson_distribution<unsigned> p_dist(p_dist_mean);
    cout << "   sample distribution = " << dist_name << endl;
    
    for (int i = 0; i < training_data_sz; i++) {
        training_input_arr[i] = p_dist(generator);
        training_output_arr[i] = rule(training_input_arr[i]);
        //cout << training_input_arr[i] << endl;
    }
    // print untrained output
    cout << "untrained output for input of "<< sample_input << ": " << test_network(sample_input) << endl;
    // train the network
    cout << "Annealing parameters: " << endl
         << "   T0 = " << T0 << endl
         << "   Tf = " << Tf << endl
         << "Beginning training.. " << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < training_data_sz; i++) {
        test_network.anneal(training_input_arr[i], training_output_arr[i], T0, Tf);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
    cout << "Training completed in " << elapsed.count() << " seconds!"<< endl;
    // generate test data
    cout << "test data size = " << test_data_sz << endl;
    unsigned int test_input_arr[test_data_sz];
    unsigned int test_output_arr[test_data_sz];
    cout << "Beginning tests.. ";
    for (int i = 0; i < test_data_sz; i++) {
        test_input_arr[i] = p_dist(generator);
        test_output_arr[i] = test_network(test_input_arr[i]);
    }
    cout << "Tests complete!" << endl;
    // compare to expectation
    float expectation_arr[test_data_sz];
    float abs_error[test_data_sz];
    float pct_error[test_data_sz];
    float mean_abs_error = 0;
    float mean_pct_error = 0;
    for (int i = 0; i < test_data_sz; i++) {
        expectation_arr[i] = rule(test_input_arr[i]);
        abs_error[i] = fabs(test_output_arr[i] - expectation_arr[i]);
        pct_error[i] = abs_error[i]/fabs(expectation_arr[i]) * 100;
        mean_abs_error += abs_error[i]/test_data_sz;
        mean_pct_error += pct_error[i]/test_data_sz;
    }
    // print results
    cout << "Mean absolute error: " << mean_abs_error << endl
         << "Mean percentage error: " << mean_pct_error << endl
         << "Trained output for sample input of " << sample_input << ": " << test_network(sample_input) << endl;
}
