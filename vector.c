#include "vector.h"
#include <stdlib.h>

vector* new_vector(int size){
    vector* new = (vector*)malloc(sizeof(vector));
    //init every value with 0
    new->values = (float*)calloc(sizeof(float), size);
    new->size = size;
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
    for(int i = 0; i < v1->size; i++)
        v1->values[i] += v2->values[i];
}

void v_sub(vector* v1, vector* v2){
    for(int i = 0; i < v1->size; i++)
        v1->values[i] -= v2->values[i];
}

void v_mul(vector* v1, vector* v2){
    for(int i = 0; i < v1->size; i++)
        v1->values[i] *= v2->values[i];
}

void v_mul_v(vector* v, float value){
    for(int i = 0; i < v->size; i++)
        v->values[i] *= value;
}

void v_div(vector* v1, vector* v2){
    for(int i = 0; i < v1->size; i++)
        v1->values[i] /= v2->values[i];
}

vector* v_m_mul(matrix* m, vector* v){
    vector* new = (vector*)malloc(sizeof(vector));
    new->size = m->height;
    new->values = (float*)malloc(sizeof(float) * m->height);
    for(int x = 0; x < m->width; x++)
        for(int y = 0; y < m->height; y++)
            new->values[y] = m->values[x][y] * v->values[x];
    return new;
}

void v_foreach_a(vector* v, float(*action)()){
    for(int i = 0; i < v->size; i++)
        v->values[i] = action();
}

void v_foreach_f(vector* v, float(*func)(float)){
    for(int i = 0; i < v->size; i++)
        v->values[i] = func(v->values[i]);
}

void m_foreach_a(matrix* m, float(*action)()){
    for(int i = 0; i < m->width; i++)
        for(int j = 0; j < m->height; j++)
            m->values[i][j] = action();
}   

void m_foreach_f(matrix* m, float(*func)(float)){
    for(int i = 0; i < m->width; i++)
        for(int j = 0; j < m->height; j++)
            m->values[i][j] = func(m->values[i][j]);
}

void v_free(vector* v){
    free(v->values);
    free(v);
}

void m_free(matrix* m){
    for(int x = 0; x < m->width; x++)
        free(m->values[x]);
    free(m->values);
    free(m);
}