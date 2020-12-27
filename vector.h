#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector{
    float* values;
    int size;
}vector;

typedef struct matrix{
    int width;
    int height;
    float** values;
}matrix;

void v_add(vector* v1, vector* v2);
void v_sub(vector* v1, vector* v2);
void v_mul(vector* v1, vector* v2);
void v_div(vector* v1, vector* v2);
void v_foreach_f(vector* v, float(*func)(float));
void v_foreach_a(vector* v, float(*action)());
void m_foreach_f(matrix* v, float(*func)(float));
void m_foreach_a(matrix* v, float(*action)());
vector* v_m_mul(matrix* m, vector* v);

#endif