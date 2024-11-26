#ifndef __MADGWICK_H
#define __MADGWICK_H
#include "Quaternion.h"
#include "GY86.h"

void MadgwickAHRSupdate(Quaternion *q,gy_t * gy86_data,float* a, float* b, float* c);

#endif 