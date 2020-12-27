#include <stdio.h>
#include "network.h"

float acitivation(float value);
float deriv_activation(float value);

float acitivation(float value){
    return value;
}

float deriv_activation(float value){
    return 1;
}

int main(int argc, char** argv){
    int size[] = {784, 16, 16, 10};
    network* net = create_network(size, 4, acitivation, deriv_activation);
    network_free(net);
}

