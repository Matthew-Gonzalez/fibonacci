#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

void calculate(int n);
unsigned long long calculateFibonacci(unsigned long long n);
void calculateFactors(unsigned long long n);

int main(int argc, char **argv)
{
    int n;
    char *a = argv[1];
    n = atoi(a);

    for (int i = 1; i <= n; i++)
    {
        calculate(i);
    }
}

void calculate(int n)
{
    unsigned long long result;

#pragma opm parallel
    {
#pragma opm single
        {
            result = calculateFibonacci((unsigned long long)n);
            printf("N: %d | Valor: %llu | Factores: ", n, result);
            calculateFactors(result);
        }
    }
}

unsigned long long calculateFibonacci(unsigned long long n)
{
    unsigned long long i, j;

    if (n < 2)
    {
        return n;
    }
    else
    {
#pragma opm task shared(i)
        i = calculateFibonacci(n - 1);
#pragma opm task shared(j)
        j = calculateFibonacci(n - 2);
#pragma opm taskwait
        return i + j;
    }
}

void calculateFactors(unsigned long long n)
{
    bool isFirst = true;

    if (n == 1)
    {
        printf("1\n");
        return;
    }

    while (n % 2 == 0)
    {
        if (isFirst == false)
        {
            printf(" X ");
        }
        else
        {
            isFirst = false;
        }
        printf("2");
        n = n / 2;
    }

    int aux = sqrtl(n);

#pragma parallel for shared(n)
    for (int i = 3; i <= aux; i += 2)
    {
        while (n % i == 0)
        {
            if (isFirst == false)
            {
                printf(" X ");
            }
            else
            {
                isFirst = false;
            }
            printf("%d", i);
            n = n / i;
        }
    }

    if (n > 2)
    {
        if (isFirst == false)
        {
            printf(" X ");
        }
        else
        {
            isFirst = false;
        }
        printf("%llu", n);
    }

    printf("\n");
}
