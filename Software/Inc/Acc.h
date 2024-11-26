#ifndef __ACC_H
#define __ACC_H

typedef struct {
    float x;
    float y;
    float z;
} Vector3f_t;

float vector_norm(Vector3f_t v);

float compute_error(Vector3f_t input, Vector3f_t offset, Vector3f_t scale, float target_norm);

void gradientDescent(Vector3f_t accData[],Vector3f_t magData[],int dataSize,Vector3f_t* offset_a,Vector3f_t* scale_a,Vector3f_t* offset_m,Vector3f_t* scale_m);

void SO_Init(Vector3f_t * offset_a, Vector3f_t* offset_g ,Vector3f_t* offset_m, Vector3f_t* scale_a, Vector3f_t* scale_m);


#endif
