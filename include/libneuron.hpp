namespace LibNeuron {
    class Edge;
    class Network;
    class Neuron {
        class Impl;
        Impl* pimpl;
    public:
        // default constructor
        Neuron();
        // constructor to connect to an array of neurons
        Neuron(Neuron* neuron_arr, unsigned neuron_arr_size);
        unsigned get_size() const;
        Edge* get_edges() const;
        // get the unprocessed input signal
        float get_input_signal() const;
        void set_edges(Neuron*, unsigned);
        void set_edges(Edge*, unsigned);
        // add the argument to the input signal
        void signal_add(float);
        // reset the input signal to 0
        void reset_signal();
        // a non-linear function that processes the signal
        float activation();
        // a function to send the processed signal to output edges
        // pre-conditions:
            // all input edges have passed their signals
        // post-conditions:
            // this has transmitted its processed signal to the output edges
            // input_signal == 0
        void fire();        
        // use simulated annealing to train the current neuron
        // pre-conditions:
            // input and output neurons are valid neuron ptrs 
            // this is a neuron in a feed-forward network starting with a single input neuron and ending with a single output neuron
        // post-conditions:
            // the weights of the output edges have been changed via simulated annealing
        void metropolis(const Network& arg_network, unsigned arg_input_signal, unsigned expectation, float temperature);
        Neuron& operator=(const Neuron&);
        ~Neuron();
    };
}
