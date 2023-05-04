namespace LibNeuron {
    class Neuron;
    class Edge {
        class Impl;
        Impl* pimpl;
    public:
        // default constructor
        Edge();
        // contructor to connect to a Neuron and set weight
        // pre-conditions:
            // ptr arg is valid
        // post-conditions:
            // tip == argument
        Edge(LibNeuron::Neuron*, float); 
        // getters
        float get_weight(); 
        // setters
        void set_weight(float);
        void set_tip(const LibNeuron::Neuron&);
        // sends the weighted argument signal to the tip Neuron
        void transmit(float);
        Edge& operator=(const Edge&);
        ~Edge();
    };
}
