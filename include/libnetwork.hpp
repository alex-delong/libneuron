namespace LibNeuron {
    class Layer;
    class Network {
        class Impl;
        Impl* pimpl;
    public:
        Network();
        Network(const Network&);
        unsigned get_size() const;
        const Layer& get_layer(unsigned) const;
        float*** get_weights() const;
        // initialize an array of sz layers, each with a size given by layer_sz_arr
        Network(int* layer_sz_arr, int sz);
        // perform simulated annealing to train the network
        void anneal(float arg_input_signal, float expectation, float T0, float Tf);
        void anneal(bool* arg_input_signal, bool expectation, float T0, float Tf);
        unsigned operator()(unsigned) const;
        bool* operator()(bool*) const;
        ~Network();
    };
}
