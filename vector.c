#include "vector.h"
#include <stdlib.h>

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
}

void v_foreach_a(vector* v, float(*action)()){
    for(int i = 0; i < v->size; i++)
        v->values[i] = action();
}

void v_foreach_f(vector* v, float(*func)(float)){
    for(int i = 0; i < v->size; i++)
        v->values[i] = func(v->values[i]);
}

void v_foreach_a(matrix* m, float(*action)()){
    for(int i = 0; i < m->width; i++)
        for(int j = 0; j < m->height; m++)
            m->values[i][j] = action();
}

void v_foreach_a(matrix* m, float(*func)(float)){
    for(int i = 0; i < m->width; i++)
        for(int j = 0; j < m->height; m++)
            m->values[i][j] = action(m->values[i][j]);
}
