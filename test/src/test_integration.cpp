#include <libneuron.hpp>
#include <random>
#include <cmath>
#include <iostream>

int main() {
    using std::cout;
    using std::endl;
    using namespace LibNeuron;
    // make a feed-forward neural network with input size == output size == 1 and 1 hidden layer of size 5
    const int num_inputs = 1;
    const int num_outputs = 1;
    const int num_hidden = 1;
    const int num_layers = num_hidden + 2;
    cout << "Test network parameters: " << endl
         << "   inputs = " << num_inputs << endl
         << "   outputs = " << num_outputs << endl
         << "   layers = " << num_layers << endl;


    int layer_sz_arr[num_layers] = {num_inputs, 5, num_outputs};
    Network test_network(layer_sz_arr, num_layers);
    // make training data
    // rule: division by 10
    const char* rule_str = "division by 10";
    cout << "Training parameters: " << endl
         << "   rule = " << rule_str << endl;

    auto rule = [](float x){
        return x/10.0;
    };
    int training_data_sz = 100000;
    cout << "   training data size = " << training_data_sz << endl;

    float training_input_arr[training_data_sz];
    float training_output_arr[training_data_sz];
    std::default_random_engine generator;
    std::normal_distribution<float> norm_dist;
    cout << "   sample distribution = normal" << endl;
    
    for (int i = 0; i < training_data_sz; i++) {
        training_input_arr[i] = norm_dist(generator);
        training_output_arr[i] = rule(training_input_arr[i]);
    }
    // print untrained output
    cout << "untrained output: 5 / 10 = " << test_network(5.0) << endl;
    // train the network
    const float T0 = 10.0;
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
    float test_input_arr[test_data_sz];
    float test_output_arr[test_data_sz];
    cout << "Beginning tests.. ";
    for (int i = 0; i < test_data_sz; i++) {
        test_input_arr[i] = norm_dist(generator);
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
        expectation_arr[i] = test_input_arr[i]/10.0;
        abs_error[i] = fabs(test_output_arr[i] - expectation_arr[i]);
        pct_error[i] = abs_error[i]/fabs(expectation_arr[i]) * 100;
        mean_abs_error += abs_error[i]/test_data_sz;
        mean_pct_error += pct_error[i]/test_data_sz;
    }
    // print results
    cout << "Mean absolute error: " << mean_abs_error << endl
         << "Mean percentage error: " << mean_pct_error << endl
         << "Example output: 5 / 10 = " << test_network(5.0) << endl;
}
