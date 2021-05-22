#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "network.h"

#define E 2.71828182846f

void acitivation(float* value);
void deriv_activation(float* value);

void acitivation(float* value){
    *value =  1.0f / (1 + pow(E, -*value));
}

void deriv_activation(float* value){
    *value = *value * (1 - *value);
}

int main(int argc, char** argv){
    FILE* fp = fopen("data.csv", "r");
    int length = 0;
    vector** input = NULL;
    vector** output = NULL;
    printf("Start reading training data\n");
    int in;
    while(fscanf(fp, "%i", &in) != EOF){
        if(length >= 10)
            break;

        length++;
        input = (vector**)realloc(input, sizeof(vector*) * length);
        output = (vector**)realloc(output, sizeof(vector*) * length);
        vector* new_input = new_vector(784, NO_ZERO);
        vector* new_output = new_vector(10, NONE);
        new_output->values[in] = 1;
        input[length - 1] = new_input;
        output[length - 1] = new_output;
        new_input->values[0] = in;
        for(int i = 0; i < 784; i++){
            fscanf(fp, "%i", &in);
            v_insert(new_input, in / 255.0f, i);
        }
    }
    
    fclose(fp);
    printf("finished reading training data: %i samples\n", length);
    int size[] = {784, 16, 16, 10};
    network* net = create_network(size, 4, acitivation, deriv_activation);
    for(int k = 0; k < 50; k++){
        double delta;
        float loss = 0;
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for(int i = 0; i < length; i++){
            vector* net_in = input[i];
            vector* net_out = output[i];
            loss += backward(net, net_in, net_out);
        }
        gettimeofday(&end, NULL);
        delta =  (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        printf("Epoch: %i Time: %f Acc: %f %ius/step\n", k + 1, delta / 1000000, loss / (float)length, (int)(delta / length));
    }

    for(int i = 0; i < length; i++){
        v_free(input[i]);
        v_free(output[i]);
    }
    free(input);
    free(output);
    network_free(net);
    return 0;
}

