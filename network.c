#include <stdlib.h>
#include "network.h"

layer* create_layer(int input_size, int output_size, float (*activation)(float), float(*deriv_a)(float)){
    layer* new = (layer*)malloc(sizeof(layer));

    vector* bias = (vector*)malloc(sizeof(vector));
    bias->size = output_size;
    bias->values = (float*)malloc(sizeof(float) * output_size);
    v_foreach_a(bias, random);

    matrix* weights = (matrix*)malloc(sizeof(matrix));
    weights->width = input_size;
    weights->height = output_size;
    weights->values = (float**)malloc(sizeof(float*) * input_size);
    for(int i = 0; i < input_size; i++)
        weights->values[i] = (float*)malloc(sizeof(float) * output_size);
    m_foreach_a(weights, random);

    new->bias = bias;
    new->weights = weights;
    new->activation = activation;
    new->deriv_a = deriv_a;
    return new;
}

network* create_network(float* size, int count, float (*activation)(float), float(*deriv_a)(float)){
    network* n = (network*)malloc(sizeof(network));
    n->layer = (layer**)malloc(sizeof(network*) * count);
    for(int i = 0; i < count - 1; i++, size++)
        n->layer[i] = create_layer(*size, *(size + 1), activation, deriv_a);
    n->length = count;
    return n;
}

vector* forward_layer(layer* layer, vector* input){
    vector* output = v_m_mul(input, layer->weights);
    v_add(output, layer->bias);
    v_foreach_f(output, layer->activation);
    return output;
}

vector* forward(network* net, vector* input){
    for(int i = 0; i < net->length; i++){
        vector* tmp = forward_layer(net->layer[i], input);
        if(!i)
            free(input);
        input = tmp;
    }
    return input;
}

float random(){
    return (float)rand()/(float)(RAND_MAX) * 2 - 1;
}