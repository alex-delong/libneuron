#include <libneuron.hpp>
#include <libnetwork.hpp>
#include <liblayer.hpp>
#include <libedge.hpp>
#include <random>
#include <cmath>
#include <iostream>
#include <chrono>

// network parameters
const unsigned num_inputs = 2;
const unsigned num_outputs = 1;
const unsigned num_hidden = 1;
const unsigned num_layers = num_hidden + 2;
// training parameters
const char* rule_str = "XOR";
auto rule = [](bool* arg_bits) -> bool {
    return arg_bits[0] ^ arg_bits[1];
};
const unsigned sample_input = 5;
const unsigned training_data_sz = 4;
const unsigned p_dist_mean = 1000;
const char* dist_name = "Poisson";
// annealing parameters
const float T0 = 10.0;
const float Tf = 0.0001;
// test parameters
const int test_data_sz = 4;

int main() {
    using std::cout;
    using std::endl;
    using namespace LibNeuron;
    // make network
    cout << "Test network parameters: " << endl
         << "   inputs = " << num_inputs << endl
         << "   outputs = " << num_outputs << endl
         << "   layers = " << num_layers << endl;
    int layer_sz_arr[num_layers] = {num_inputs, 3, num_outputs};
    Network test_network(layer_sz_arr, num_layers);
    // make training data
    cout << "Training parameters: " << endl
         << "   rule = " << rule_str << endl
         << "   training data size = " << training_data_sz << endl;
    
    bool** training_input_arr = new bool*[training_data_sz];
    for (int i = 0; i < training_data_sz; i++) {
        training_input_arr[i] = new bool[2];
    }
    training_input_arr[0][0] = 0;
    training_input_arr[0][1] = 0;
    training_input_arr[1][0] = 0;
    training_input_arr[1][1] = 1;
    training_input_arr[2][0] = 1;
    training_input_arr[2][1] = 0;
    training_input_arr[3][0] = 1;
    training_input_arr[3][1] = 1;

    bool training_output_arr[training_data_sz];
    //std::default_random_engine generator(std::time(nullptr));
    //std::poisson_distribution<unsigned> p_dist(p_dist_mean);
    //cout << "   sample distribution = " << dist_name << endl;
    
    for (int i = 0; i < training_data_sz; i++) {
        //training_input_arr[i] = p_dist(generator);
        training_output_arr[i] = rule(training_input_arr[i]);
    }
    // print untrained output
    //cout << "untrained output for input of "<< sample_input << ": " << test_network(sample_input) << endl;
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
    // TODO: output weights to a file
    // generate test data
    cout << "test data size = " << test_data_sz << endl;
    //unsigned test_input_arr[test_data_sz];
    //unsigned test_output_arr[test_data_sz];
    bool test_output_arr[test_data_sz];
    cout << "Beginning tests.. ";
    for (int i = 0; i < test_data_sz; i++) {
        //test_input_arr[i] = p_dist(generator);
        //test_output_arr[i] = test_network(test_input_arr[i]);
        test_output_arr[i] = test_network(training_input_arr[i]);
    }
    cout << "Tests complete!" << endl;
    // compare to expectation
    //float expectation_arr[test_data_sz];
    bool expectation_arr[test_data_sz];
    bool abs_error[test_data_sz];
    float pct_error[test_data_sz];
    float mean_abs_error = 0;
    float mean_pct_error = 0;
    for (int i = 0; i < test_data_sz; i++) {
        //expectation_arr[i] = rule(test_input_arr[i]);
        expectation_arr[i] = rule(training_input_arr[i]);
        abs_error[i] = abs(test_output_arr[i] - expectation_arr[i]);
        pct_error[i] = abs_error[i]/expectation_arr[i] * 100;
        mean_abs_error += abs_error[i]/test_data_sz;
        mean_pct_error += pct_error[i]/test_data_sz;
    }
    // print results
    cout << "Mean absolute error: " << mean_abs_error << endl
         << "Mean percentage error: " << mean_pct_error << endl;
         //<< "Trained output for sample input of " << sample_input << ": " << test_network(sample_input) << endl;
    float*** wgts = test_network.get_weights();
    for (int i = 0; i < test_network.get_size(); i++) {
        for (int j = 0; j < test_network.get_layer(i).get_size(); j++) {
            for (int k = 0; k < test_network.get_layer(i).get_neuron(j).get_size(); k++) {
                cout << wgts[i][j][k] << endl;
            }
        }
    }
}
