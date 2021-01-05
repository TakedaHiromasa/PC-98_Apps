#ifndef FPU_H
#define FPU_H

void finit();
void zeroset();

// FLOAT TYPE
float fpop();
float fadd(float a, float b);
float fmul(float a, float b);
int   fma_r(float a, float b);

// DOUBLE TYPE
double faddl(double a, double b);
double fmull(double a, double b);

#endif
