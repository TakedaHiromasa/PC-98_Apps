#ifndef FPU_H
#define FPU_H

void finit();

// FLOAT TYPE
float fadd(float a, float b);
float fmul(float a, float b);
int fma_r(float a, float b);
int zeroset();
float pop();

// DOUBLE TYPE
double faddl(double a, double b);
double fmull(double a, double b);

#endif
