#ifndef __QUATERNION_H
#define __QUATERNION_H

typedef struct {
    double a;
    double i;
    double j;
    double k;
}Quaternion;

void Quater_Init(Quaternion* Q,double a,double i,double j,double k);
void Quater_ADD(Quaternion* a, Quaternion* b);
void Quater_SUB(Quaternion* a, Quaternion* b);
void Quater_MUL(Quaternion* a, Quaternion* b);
double Quater_Magnitude(Quaternion* a);
Quaternion Quater_conjugate(Quaternion a);


#endif
