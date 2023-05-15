#define BOOST_TEST_MODULE Neuron test
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK

#include <libnetwork.hpp>
#include <libneuron.hpp>
#include <liblayer.hpp>
#include <libedge.hpp>
#include <boost/test/unit_test.hpp>
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(NeuronTestSuite)
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
BOOST_AUTO_TEST_SUITE_END()

int main(int argc, char** argv) {
    return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
