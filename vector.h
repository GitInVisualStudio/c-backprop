#ifndef VECTOR_H
#define VECTOR_H

typedef enum MODE { NONE, NO_ZERO }MODE;

typedef struct vector{
    float* values;
    int* indices;
    MODE mode; 
    int size;
}vector;

typedef struct matrix{
    int width;
    int height;
    float** values;
}matrix;

vector* new_vector(int size, MODE mode);
matrix* new_matrix(int width, int height);

void v_insert(vector* v, float value, int index);

/*
adds v1 to v2 and stores the result in v1
*/
void v_add(vector* v1, vector* v2);
/*
subtracts v2 from v1 and stores the result in v1
*/
void v_sub(vector* v1, vector* v2, float value);
/*
multiplies v1 and v2 and stores the result in v1
*/
void v_mul(vector* v1, vector* v2);
void v_mul_v(vector* v, float value);
/*
devides v1 by v2 and stores the result in v1
*/
void v_div(vector* v1, vector* v2);
/*
frees the allocated memory for the vector
*/
void v_free(void* v);
void m_free(void* m);
/*
applies the function to every element in the vector
*/
void v_foreach_f(vector* v, void (*func)(float*));
/*
applies the action to every element in the vector
*/
void v_foreach_a(vector* v, float (*action)());
void m_foreach_f(matrix* m, void (*func)(float*));
void m_foreach_a(matrix* m, float (*action)());

void v_print(vector* v);
/*
inserts the product of matrix m and vector v into out
*/
void v_m_mul(matrix* m, vector* v, vector* out);
#endif