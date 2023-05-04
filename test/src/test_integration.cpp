#include <libneuron.hpp>
#include <random>
#include <cmath>
#include <iostream>
#include <chrono>
int main() {
    using std::cout;
    using std::endl;
    using namespace LibNeuron;
    // make a feed-forward neural network with input size == output size == 1 and 1 hidden layer of size 5
    const int num_inputs = 32;
    const int num_outputs = 32;
    const int num_hidden = 1;
    const int num_layers = num_hidden + 2;
    cout << "Test network parameters: " << endl
         << "   inputs = " << num_inputs << endl
         << "   outputs = " << num_outputs << endl
         << "   layers = " << num_layers << endl;


    int layer_sz_arr[num_layers] = {num_inputs, 32, num_outputs};
    Network test_network(layer_sz_arr, num_layers);
    // make training data
    // rule: division by 10
    const char* rule_str = "division by 10";
    cout << "Training parameters: " << endl
         << "   rule = " << rule_str << endl;

    auto rule = [](unsigned int x) -> unsigned int {
        return x + 5;
    };
    const int training_data_sz = 1000;
    cout << "   training data size = " << training_data_sz << endl;

    unsigned int training_input_arr[training_data_sz];
    unsigned int training_output_arr[training_data_sz];
    std::default_random_engine generator(std::time(nullptr));
    std::poisson_distribution<unsigned int> p_dist(1000);
    cout << "   sample distribution = poisson" << endl;
    
    for (int i = 0; i < training_data_sz; i++) {
        training_input_arr[i] = p_dist(generator);
        training_output_arr[i] = rule(training_input_arr[i]);
        //cout << training_input_arr[i] << endl;
    }
    // print untrained output
    cout << "untrained output: 5 + 5 = " << test_network(5) << endl;
    // train the network
    const float T0 = 1.0;
    const float Tf = 0.01;
    cout << "Annealing parameters: " << endl
         << "   T0 = " << T0 << endl
         << "   Tf = " << Tf << endl;
    cout << "Beginning training.. ";
    for (int i = 0; i < training_data_sz; i++) {
        test_network.anneal(training_input_arr[i], training_output_arr[i], T0, Tf);
    }
    cout << "Training complete!" << endl;
    // generate test data
    const int test_data_sz = 10;
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
         << "Example output: 5 + 5 = " << test_network(5) << endl;
}
