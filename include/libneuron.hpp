namespace LibNeuron {
    class Edge;
    class Neuron;
    class Network;
    // a connection to a neuron
    class Edge {
        // TODO: put private members into Impl class
        // the neuron recieving the signal 
        Neuron* tip;
        // the coefficient of the signal
        float weight;
    public:
        // default constructor
        Edge();
        // contructor to connect to a Neuron and set weight
        // pre-conditions:
            // ptr arg is valid
        // post-conditions:
            // tip == argument
        Edge(Neuron*, float); 
        // getters
        float get_weight();
        Neuron* get_tip();
        // setters
        void set_weight(float);
        void set_tip(const Neuron&);
        // sends the weighted argument signal to the tip Neuron
        void transmit(float);
        Edge& operator=(const Edge&);
        ~Edge();
    };
 
    class Neuron {
        // TODO: put private members into Impl class
        // the unprocessed sum of the input signals
        float input_signal;
        // an array of output Edges
        Edge* output_edges;
        // output edges size
        unsigned int sz;
    public:
        // default constructor
        Neuron();
        // constructor to connect to an array of neurons
        Neuron(Neuron*, unsigned int);
        // output edges setter
        void set_edges(Neuron*, unsigned int);
        // add the argument to the input signal
        void signal_add(float);
        // a non-linear function that processes the signal
        float activation();
        // a function to send the processed signal to output edges
        // pre-conditions:
            // all input edges have passed their signals
        // post-conditions:
            // this has transmitted its processed signal to the output edges
            // input_signal == 0
        void fire();
        // get the unprocessed input signal
        float read_signal();
        void reset_signal();
        // output edges getter
        Edge* get_edges();
        unsigned int get_size();
        // use simulated annealing to train the current neuron
        // pre-conditions:
            // input and output neurons are valid neuron ptrs 
            // this is a neuron in a feed-forward network starting with a single input neuron and ending with a single output neuron
        // post-conditions:
            // the weights of the output edges have been changed via simulated annealing
        void metropolis(Network arg_network, float arg_input_signal, float expectation, float temperature);
        Neuron& operator=(const Neuron&);
        ~Neuron();
    };
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
        float operator()(float);
        ~Network();
    };
}


