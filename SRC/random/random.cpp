#include "random.h"
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

#define RAN 1220703125
#define MAXU INT_MAX
#define NextU (((unsigned long) (U = (U*RAN)|1))>>1) 
#define Random ((NextU + 0.5) / (MAXU + 1.0))

static void Error(const char *Message) {
    std::cerr << "Error: " << Message << std::endl; 
    exit(0);
}

int Draw(double A, long &U) {
    return Random < A;
}

long Randint(long A, long B, long &U) {
    if (B < A)
        Error("Randint: Second parameter is lower than first parameter");
    long u = NextU;
    double R;
    u = (long) (R = u*(B - A + 1.0)/(MAXU + 1.0) + A);
    return u > R ? u-1 : u;
}

double Uniform(double A, double B, long &U) {
    if (B < A)
        Error("Uniform: Second parameter is lower than first parameter");
    return Random*(B-A) + A;
}

#define p0 (-0.322232431088)
#define p1 (-1)
#define p2 (-0.342242088547)
#define p3 (-0.0204231210245)
#define p4 (-0.0000453642210148)
#define q0 0.099348462606
#define q1 0.588581570495
#define q2 0.531103462366
#define q3 0.10353775285
#define q4 0.0038560700634

double Normal(double A, double B, long &U) {
    double y, x, p, R = Random;
    p = R > 0.5 ? 1.0 - R : R;
    y = sqrt (-log (p * p));
    x = y + ((((y * p4 + p3) * y + p2) * y + p1) * y + p0) / 
            ((((y * q4 + q3) * y + q2) * y + q1) * y + q0);
    if (R < 0.5)
        x = -x;
    return B * x + A;
}

double Negexp(double A, long &U) {
    if (A <= 0)
        Error("Negexp: First parameter is lower than zero");
    return -log(Random)/A;
}

long Poisson(double A, long &U) {
    double Limit = exp(-A), Prod = NextU;
    long n;
    for (n = 0; Prod >= Limit; n++)
        Prod *= Random;
    return n;
}

double Erlang(double A, double B, long &U) {
    if (A <= 0)
        Error("Erlang: First parameter is not greater than zero "); 
    if (B <= 0)
        Error("Erlang: Second parameter is not greater than zero"); 
    long Bi = (long) B, Ci;
    if (Bi == B)
        Bi--;
    double Sum = 0;
    for (Ci = 1; Ci <= Bi; Ci++)
        Sum += log(Random);
    return (-(Sum + (B - (Ci-1))*log(Random))/(A*B)); 
}

long Discrete(double A[], long N, long &U) {
    double Basic = Random;
    long i;
    for (i = 0; i < N; i++)
        if (A[i] > Basic)
            break;
    return i;
}

double Linear(double A[], double B[], long N, long &U) {
    if (A[0] != 0.0 || A[N-1] != 1.0)
        Error("Linear: Illegal value in first array");
    double Basic = Random;
    long i;
    for (i = 1; i < N; i++)
        if (A[i] >= Basic)
            break;
    double D = A[i] - A[i-1];
    if (D == 0.0)
        return B[i-1];
    return B[i-1] + (B[i]-B[i-1])*(Basic-A[i-1])/D; 
}

long Histd(double A[], long N, long &U) {
    double Sum = 0.0;
    long i;
    for (i = 0; i < N; i++)
        Sum += A[i];
    double Weight = Random * Sum;
    Sum = 0.0;
    for (i = 0; i < N - 1; i++) {
        Sum += A[i];
        if (Sum >= Weight)
            break;
    }
    return i;
}