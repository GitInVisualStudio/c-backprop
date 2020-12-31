#include <stdlib.h>
#include <stdio.h>
#include "network.h"

layer* create_layer(int input_size, int output_size, float (*activation)(float), float (*deriv_a)(float)){
    layer* new = (layer*)malloc(sizeof(layer));

    vector* biases = new_vector(output_size);
    v_foreach_a(biases, random_f);

    matrix* weights = new_matrix(input_size, output_size);
    m_foreach_a(weights, random_f);

    new->biases = biases;
    new->weights = weights;
    new->activation = activation;
    new->deriv_a = deriv_a;
    new->output = NULL;
    new->input = NULL;
    return new;
}

network* create_network(int* size, int count, float (*activation)(float), float (*deriv_a)(float)){
    network* n = (network*)malloc(sizeof(network));
    n->layer = (layer**)malloc(sizeof(network*) * count - 1);
    for(int i = 0; i < count - 1; i++, size++){
        n->layer[i] = create_layer(*size, *(size + 1), activation, deriv_a);
    }
    n->length = count - 1;
    return n;
}

vector* forward_layer(layer* layer, vector* input){
    layer->input = input;
    vector* output = v_m_mul(layer->weights, input);
    v_add(output, layer->biases);
    v_foreach_f(output, layer->activation);
    layer->output = output;
    return output;
}

vector* forward(network* net, vector* input){
    for(int i = 0; i < net->length; i++){
        input = forward_layer(net->layer[i], input);;
    }
    return input;
}

vector* backward_layer(layer* layer, vector* error){
    //if the layer hasnt calcuated anything yet
    if(!layer->output || !layer->activation){
        perror("There is no output or activation for this layer. You have to go forward through the network first, inorder to backpropagate.\n");
        return NULL;
    }
    v_foreach_f(layer->output, layer->deriv_a);
    //output is stored in error
    v_mul(error, layer->output);

    /*
    TODO:   -implement a smooth way to store the different deriv of the biases and weights
            -add a way to choose an optimiser  
    */
    //for now on the learning rate and optimiser are hardcoded
    float learning_rate = 0.1f;
    v_mul_v(layer->biases, learning_rate);
    v_add(layer->biases, error);

    vector* deriv_activation = (vector*)malloc(sizeof(vector));
    deriv_activation->values = (float*)malloc(sizeof(float) * layer->input->size);
    deriv_activation->size = layer->input->size;
    for(int x = 0; x < layer->weights->width; x++){
        for(int y = 0; y < layer->weights->height; y++){
            //first calculate the deriv befor changing the weights
            deriv_activation->values[x] = layer->weights->values[x][y] * error->values[y];
            layer->weights->values[x][y] += error->values[y] * layer->input->values[x];
        }
    }

    //we dont need the output of the layer anymore for the network
    //because we free the output of this layer, the activation of the next layer is also freed, so we dont need to worry about that
    v_free(layer->output);
    return deriv_activation;
}

float backward(network* net, vector* input, vector* output){
    //first feed the input into the network
    vector* error = forward(net, input);
    v_sub(error, output);
    float loss = 0;
    for(int i = 0; i < error->size; i++)
        loss += error->values[i] * error->values[i];
    // //go in reverse order through the network
    for(int i = net->length - 1; i >= 0; i--){
        vector* tmp = backward_layer(net->layer[i], error);
        //the first error is the output of the last layer, thats why we dont need to free the first error
        if(i != net->length - 1)
            v_free(error);
        error = tmp;
    }
    v_free(error);
    return loss;
}

float random_f(){
    return (float)rand()/(float)(RAND_MAX) * 2 - 1;
}

void network_free(network* net){
    for(int i = 0; i < net->length; i++){
        m_free(net->layer[i]->weights);
        v_free(net->layer[i]->biases);
        free(net->layer[i]);
    }
    free(net->layer);
    free(net);
}