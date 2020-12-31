#include <stdio.h>
#include "network.h"
#include <math.h>

#define E 2.71828182846f

float acitivation(float value);
float deriv_activation(float value);

float acitivation(float value){
    return 1.0f / (1 + powf(E, value));
}

float deriv_activation(float value){
    return value * (1 - value);
}

int main(int argc, char** argv){
    vector* inputs[10];
    for(int i = 0; i < 10; i++)
    {
        vector* n = new_vector(4);
        v_foreach_a(n, random_f);
        inputs[i] = n;
    }
    vector* output = new_vector(1);
    output->values[0] = 0.5f;
    int size[] = {4, 4, 1};
    network* net = create_network(size, 3, acitivation, deriv_activation);

    for(int i = 0; i < 100; i++){
        float loss = backward(net, inputs[0], output);
        printf("Loss: %f\n", loss);
    }

    for(int i = 0; i < 10; i++)
        v_free(inputs[i]);

    v_free(output);
    network_free(net);
}

