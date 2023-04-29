namespace LibNeuron {
    class Edge;
    class Neuron;
    // a connection to a neuron
    class Edge {
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
        
        
        // weight setter
        void set_weight(float);
        // tip setter
        void set_tip(const Neuron&);
        // sends the weighted argument signal to the tip Neuron
        void transmit(float);
        ~Edge();
    };
 
    class Neuron {
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
        ~Neuron();
    };
}
