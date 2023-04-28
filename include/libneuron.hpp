class Edge;
class Neuron;

// a connection to a neuron
class Edge {
    Neuron* tip;
    float weight;
public:
    // contructor to connect to a neuron
    Edge(Neuron*);
    // transmit signal to tip
    void transmit(float);
};

// 
class Neuron {
    Edge* edges;
public:
    // constructor to connect to an array of neurons
    Neuron(Neuron*);
    // a non-linear function for recieving inputs
    void activate();
};
