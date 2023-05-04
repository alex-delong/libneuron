namespace LibNeuron {
    class Network {
        class Impl;
        Impl* pimpl;
    public:
        Network();
        Network(const Network&);
        // initialize an array of sz layers, each with a size given by layer_sz_arr
        Network(int* layer_sz_arr, int sz);
        // perform simulated annealing to train the network
        void anneal(float arg_input_signal, float expectation, float T0, float Tf);
        unsigned int operator()(unsigned int) const;
        ~Network();
    };
}
