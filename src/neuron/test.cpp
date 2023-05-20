#define BOOST_TEST_MODULE Neuron test
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK

#include <network/network.hpp>
#include <neuron/neuron.hpp>
#include <layer/layer.hpp>
#include <edge/edge.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_parameters.hpp>
#include <stdexcept>
#include <random>
#include <neuron/impl.hpp>

struct TestImpl : public NeuronImpl {
    TestImpl() : NeuronImpl() {}
    TestImpl(Neuron* arg_neuron_arr, unsigned arg_sz) : NeuronImpl(arg_neuron_arr, arg_sz) {}
    void test_get_edges() {
        if (this->output_edges != this->get_edges()) {
            throw std::logic_error("get_edges() did not return the correct value");
        }
    }
    void test_get_size() {
        if (this->sz != this->get_size()) {
            throw std::logic_error("get_size() did not return the correct value");
        }
    }
    void test_get_weights() {
        double* test_out = this->get_weights();
        for (unsigned i = 0; i < this->sz; i++) {
            if (test_out[i] != this->output_edges[i].get_weight()) {
                throw std::logic_error("get_weights() did not return the correct value");
            }
        }
        delete[] test_out;
    }
    void test_get_bias() {
        if (this->bias != this->get_bias()) {
            throw std::logic_error("get_bias() did not return the correct value");
        }
    }
    void test_get_input_signal() {
        if (this->input_signal != this->get_input_signal()) {
            throw std::logic_error("get_input_signal() did not return the correct value");
        }
    }
};


BOOST_AUTO_TEST_SUITE(NeuronTestSuite)

BOOST_AUTO_TEST_CASE(getters) {
    TestImpl impl;
    impl.test_get_edges();
    impl.test_get_size();
    //impl.test_get_weights();
    impl.test_get_bias();
    impl.test_get_input_signal();    
}

BOOST_AUTO_TEST_CASE(constructors_test) {
    LibNeuron::Neuron neuron_default;
    BOOST_CHECK_EQUAL(neuron_default.get_edges(), nullptr);
    BOOST_CHECK_EQUAL(neuron_default.get_size(), 0);
    BOOST_CHECK_THROW(neuron_default.get_weights(), std::logic_error);
    BOOST_CHECK_EQUAL(neuron_default.get_bias(), 0);
    BOOST_CHECK_EQUAL(neuron_default.get_input_signal(), 0); 

    LibNeuron::Neuron neuron_arr[5];
    LibNeuron::Neuron neuron_arr_constr(neuron_arr, 5);
    BOOST_CHECK_NE(neuron_arr_constr.get_edges(), nullptr);
    BOOST_CHECK_EQUAL(neuron_arr_constr.get_size(), 5);
    double* wgts_arr_constr = neuron_arr_constr.get_weights();
    BOOST_CHECK_NE(wgts_arr_constr, nullptr);
    delete[] wgts_arr_constr;
    BOOST_CHECK_EQUAL(neuron_arr_constr.get_bias(), 0);
    BOOST_CHECK_EQUAL(neuron_arr_constr.get_input_signal(), 0);
}

BOOST_AUTO_TEST_CASE(setters) {
    std::default_random_engine generator;
    std::normal_distribution<double> norm_dist;

    LibNeuron::Neuron neuron_default;
    LibNeuron::Neuron neuron_arr[8];
    neuron_default.set_edges(neuron_arr, 8);
    LibNeuron::Edge* edges_default = neuron_default.get_edges();
    BOOST_CHECK_NE(edges_default, nullptr);
    BOOST_CHECK_EQUAL(neuron_default.get_size(), 8);
    
    LibNeuron::Edge* edges_default_other = new LibNeuron::Edge[neuron_default.get_size()];
    std::copy(
        neuron_default.get_edges(),
        neuron_default.get_edges() + neuron_default.get_size(),
        edges_default_other
    );
    LibNeuron::Neuron neuron_default_other;
    neuron_default_other.set_edges(edges_default_other, neuron_default.get_size()); 
    BOOST_CHECK_NE(neuron_default_other.get_edges(), nullptr);
    BOOST_CHECK_EQUAL(neuron_default_other.get_size(), neuron_default.get_size());
   
    double rand_sig;
    double total_sig = 0;
    for (unsigned i = 0; i < 10000; i++) {
        rand_sig = norm_dist(generator);
        total_sig += rand_sig;
        neuron_default.signal_add(rand_sig);
        BOOST_CHECK_EQUAL(neuron_default.get_input_signal(), total_sig);
    }

    neuron_default.reset_signal();
    BOOST_CHECK_EQUAL(neuron_default.get_input_signal(), 0);

    BOOST_CHECK_THROW(neuron_default.set_weights(nullptr), std::invalid_argument);
    double* wgts_arr = new double[neuron_default.get_size()];
    for (unsigned i = 0; i < neuron_default.get_size(); i++) {
        wgts_arr[i] = norm_dist(generator);
    }
    neuron_default.set_weights(wgts_arr);

    double rand_bias;
    for (unsigned i = 0; i < 10000; i++) {
        rand_bias = norm_dist(generator);
        neuron_default.set_bias(rand_bias);
        BOOST_CHECK_EQUAL(neuron_default.get_bias(), rand_bias);
    } 
}

BOOST_AUTO_TEST_CASE(fire) {
    LibNeuron::Neuron default_neuron;
    LibNeuron::Neuron neuron_arr[5];
    double* wgts_arr = new double[5]{1.0, 1.0, 1.0, 1.0, 1.0};
    default_neuron.set_edges(neuron_arr, 5);
    default_neuron.set_weights(wgts_arr);
    default_neuron.signal_add(5.0);
    default_neuron.fire();
    BOOST_CHECK_EQUAL(default_neuron.get_input_signal(), 0);
    for (unsigned i = 0; i < 5; i++) {
        BOOST_CHECK_NE(neuron_arr[i].get_input_signal(), 0);
    }
    
}

BOOST_AUTO_TEST_SUITE_END()

int main(int argc, char** argv) {
    return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
