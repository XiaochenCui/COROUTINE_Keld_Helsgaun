#ifndef RANDOM_H
#define RANDOM_H

int Draw(double A, long &U);

long Randint(long A, long B, long &U);

double Uniform(double A, double B, long &U);

double Normal(double A, double B, long &U);

double Negexp(double A, long &U);

long Poisson(double A, long &U);

double Erlang(double A, double B, long &U);

long Discrete(double A[], long N, long &U);

double Linear(double A[], double B[], long N, long &U);

long Histd(double A[], long N, long &U);

#endif