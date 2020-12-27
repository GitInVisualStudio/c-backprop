#ifndef NETWORK_H
#define NETWORK_H

#include "vector.h"

typedef struct layer{
    vector* bias;
    matrix* weights;
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
network* create_network(float* size, int count, float (*activation)(float), float(*deriv_a)(float));
layer* create_layer(int input_size, int output_size, float (*activation)(float), float(*deriv_a)(float));
vector* forward(network* net, vector* input);
vector* forward_layer(layer* layer, vector* input);
vector* backward(vector* output);
vector* backward_layer(layer* layer, vector* output);
float random();
#endif