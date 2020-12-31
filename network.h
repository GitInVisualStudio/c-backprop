#ifndef NETWORK_H
#define NETWORK_H

#include "vector.h"

typedef struct layer{
    vector* biases;
    matrix* weights;
    //used when we backpropagate, so that we dont need to calcualte the output of the layer twice
    vector* output;
    vector* input;
    float (*activation)(float);
    float (*deriv_a)(float);
}layer;

typedef struct network{
    layer** layer;
    int length;
}network;

/*
size is an array of the input size for each layer
*/
network* create_network(int* size, int count, float (*activation)(float), float(*deriv_a)(float));
layer* create_layer(int input_size, int output_size, float (*activation)(float), float(*deriv_a)(float));
/*
returns a new allocated vector with the output from the network
*/
vector* forward(network* net, vector* input);
/*
returns a new allocated vector with the output from the layer
*/
vector* forward_layer(layer* layer, vector* input);
/*
backpropagates through the entire network
output is the actual output what the network shoul produce
returns the error of the output
*/
float backward(network* net, vector* input, vector* output);
/*
backpropagate through the network and returns a new allocated vector with the new deriv
*/
vector* backward_layer(layer* layer, vector* error);
/*
returns a random float between -1 and 1
*/
float random_f();
void network_free(network* net);
#endif