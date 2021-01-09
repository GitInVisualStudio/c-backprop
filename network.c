#include <stdlib.h>
#include <stdio.h>
#include "network.h"
#include <math.h>

layer* create_layer(int input_size, int output_size, void (*activation)(float*), void (*deriv_a)(float*)){
    layer* new = (layer*)malloc(sizeof(layer));

    vector* biases = new_vector(output_size, NONE);
    v_foreach_a(biases, random_f);

    matrix* weights = new_matrix(input_size, output_size);
    m_foreach_a(weights, random_f);

    new->output = new_vector(output_size, NONE);
    new->deriv = new_vector(input_size, NONE);
    new->activation = activation;
    new->deriv_a = deriv_a;
    new->weights = weights;
    new->biases = biases;
    new->input = NULL;
    return new;
}

network* create_network(int* size, int count, void (*activation)(float*), void (*deriv_a)(float*)){
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
    v_m_mul(layer->weights, input, layer->output);
    v_add(layer->output, layer->biases);
    v_foreach_f(layer->output, layer->activation);
    return layer->output;
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
    v_sub(layer->biases, error);
    register int width = layer->weights->width, height = layer->weights->height;
    for(register int x = 0; x < width; x++){
        register float* arr = layer->weights->values[x];
        for(register int y = 0; y < height; y++){
            //first calculate the deriv befor changing the weights
            register float var = error->values[y];
            layer->deriv->values[x] = arr[y] * var;
            arr[y] -= var * layer->input->values[x];
        }
    }

    return layer->deriv;
}

float backward(network* net, vector* input, vector* output){
    //first feed the input into the network
    vector* current_output = forward(net, input);
    vector* error = new_vector(output->size, NONE);

    v_add(error, current_output);
    v_sub(error, output);

    float loss = 0;
    int max = 0;
    for(int i = 0; i < error->size; i++)
        if(current_output->values[i] > current_output->values[max])
            max = i;
    loss = output->values[max] == 1;
    float learning_rate = 0.1f;
    v_mul_v(error, learning_rate);

    output = error;

    //go in reverse order through the network
    for(int i = net->length - 1; i >= 0; i--)
        output = backward_layer(net->layer[i], output);
        
    v_free(error);
    return loss;
}

float random_f(){
    return (float)rand()/(float)(RAND_MAX) * 2.0f - 1.0f;
}

void network_free(network* net){
    for(int i = 0; i < net->length; i++){
        m_free(net->layer[i]->weights);
        v_free(net->layer[i]->biases);
        v_free(net->layer[i]->deriv);
        v_free(net->layer[i]->output);
        free(net->layer[i]);
    }
    free(net->layer);
    free(net);
}