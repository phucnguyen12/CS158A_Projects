#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

const int RUNCOUNT = 50;
const int N = 20;

using namespace std;
// X = X(lambda) is our random variable
double X(double lambda) {
    double u;
    do {
        u = double(rand()) / RAND_MAX;
    } while (u == 0);
    double val = -log(u) * lambda;
    return val;
    
}

double run(double lambda)
{
    int i = 0;
    double time = 0;
    double prevtime = -1;
    double nexttime = 0;
    time = X(lambda);
    nexttime = time + X(lambda);
    
    //while collision: adjacent times within +/- 1 slot
    while (time - prevtime < 1 || nexttime - time < 1) {
        prevtime = time;
        time = nexttime;
        nexttime += X(lambda);
    }
    return time;

}

int main (int argc, char * argv[]) {
    int i;
    double sum, lambda;
    double min, currentValue, max;
    for (lambda = N; lambda >= 2; lambda -= 2) {
        sum = 0;
        min = 0;
        max = 0;
        currentValue = 0;
        for (i = 0; i < RUNCOUNT; i++) {
            currentValue = run(lambda);
            
            if (i == 0) {
                min = currentValue;
                max = currentValue;
            }

            sum += currentValue;
            
            if (currentValue < min) {
                min = currentValue;
            }
            
            if (currentValue > max) {
                max = currentValue;
            }
            printf("%.3f : %.3f\n", lambda, sum/RUNCOUNT);
        }
        
        printf("%.3f : miminum value = %.3f\n", lambda, min);
        printf("%.3f : maximum value = %.3f\n", lambda, max);

    }
    return 0;
}