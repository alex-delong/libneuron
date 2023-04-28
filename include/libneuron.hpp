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
        // contructor to connect to a Neuron
        // pre-conditions:
            // argument points to a valid Neuron
        // post-conditions:
            // tip == argument
        Edge(Neuron*);
        // weight setter
        void set_weight(float);
        // sends the weighted argument signal to the tip Neuron
        void transmit(float);
        ~Edge();
    };
 
    class Neuron {
        // the unprocessed sum of the input signals
        float input_signal;
        // an array of output Edges
        Edge* output_edges;
    public:
        // constructor to connect to an array of neurons
        Neuron(Neuron*);
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
