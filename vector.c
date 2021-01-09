#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

void v_insert(vector* v, float value, int index){
    if(v->mode == NONE)
    {
        v->values[index] = value;
        return;
    }
    if(value == 0)
        return;
    v->values[index] = value;
    v->indices[v->size] = index;
    v->size++;
}

vector* new_vector(int size, MODE mode){
    vector* new = (vector*)malloc(sizeof(vector));
    //init every value with 0
    new->values = (float*)calloc(sizeof(float), size);
    new->size = size;
    new->mode = mode;
    if(mode == NONE)
        return new;
    new->indices = (int*)malloc(sizeof(int) * size);
    //if the vector only stores none zero values, the size becomes the length of the indices array
    new->size = 0;
    return new;
}

matrix* new_matrix(int width, int height){
    matrix* new = (matrix*)malloc(sizeof(matrix));
    new->width = width;
    new->height = height;
    new->values = (float**)malloc(sizeof(float*) * width);
    for(int i = 0; i < width; i++)
        new->values[i] = (float*)calloc(sizeof(float), height);
    return new;
}

void v_add(vector* v1, vector* v2){
    register int size = v1->size;
    for(register int i = 0; i < size; i++)
        v1->values[i] += v2->values[i];
}

void v_sub(vector* v1, vector* v2){
    register int size = v1->size;
    for(register int i = 0; i < size; i++)
        v1->values[i] -= v2->values[i];
}

void v_mul(vector* v1, vector* v2){
    register int size = v1->size;
    for(register int i = 0; i < size; i++)
        v1->values[i] *= v2->values[i];
}

void v_mul_v(vector* v, float value){
    register int size = v->size;
    for(register int i = 0; i < size; i++)
        v->values[i] *= value;
}

void v_div(vector* v1, vector* v2){
    register int size = v1->size;
    for(register int i = 0; i < size; i++)
        v1->values[i] /= v2->values[i];
}

void v_m_mul(matrix* m, vector* v, vector* out){
    register int width = m->width, height = m->height;
    switch(v->mode){
        case NONE:
        for(register int x = 0; x < width; x++){
            register float* arr = m->values[x];
            register float var = v->values[x];
            for(register int y = 0; y < height; y++){
                //delete all prev values in the out vector
                if(!x)
                    out->values[y] = 0;
                out->values[y] += arr[y] * var;
            }
        }
        break;
        case NO_ZERO:
        for(register int i = 0; i < v->size; i++){
            register int index = v->indices[i];
            register float* arr = m->values[index];
            register float var = v->values[index];
            for(register int y = 0; y < height; y++){
                if(!i)
                    out->values[y] = 0;
                out->values[y] += arr[y] * var;
            }
        }
        break;
        default:
        return;
    }
}

void v_foreach_a(vector* v, float (*action)()){
    for(int i = 0; i < v->size; i++)
        v->values[i] = action();
}

void v_foreach_f(vector* v, void (*func)(float*)){
    register int size = v->size;
    for(register int i = 0; i < size; i++)
        func(&v->values[i]);
}

void m_foreach_a(matrix* m, float (*action)()){
    for(int i = 0; i < m->width; i++)
        for(int j = 0; j < m->height; j++)
            m->values[i][j] = action();
}   

void m_foreach_f(matrix* m, void (*func)(float*)){
    for(int i = 0; i < m->width; i++)
        for(int j = 0; j < m->height; j++)
            func(&m->values[i][j]);
}

void v_free(void* v){
    vector* ve = (vector*)v;
    if(ve->mode == NO_ZERO)
        free(ve->indices);
    free(ve->values);
    free(v);
}

void m_free(void* m){
    matrix* ma = (matrix*)m;
    for(int x = 0; x < ma->width; x++)
        free(ma->values[x]);
    free(ma->values);
    free(m);
}

void v_print(vector* v){
    printf("Size: %i Values: ", v->size);
    for(int i = 0; i < v->size; i++)
        printf("%f, ", v->values[i]);
    printf("\n");
}