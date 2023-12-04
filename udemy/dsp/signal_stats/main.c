#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

extern double InputSignal_f32_1kHz_15kHz[320];

double signal_calc_mean(double *signal, size_t len);
double signal_calc_variance(double *signal, size_t len, double signal_mean);
double signal_calc_std(double variance);

int main(void)
{
    size_t len = sizeof(InputSignal_f32_1kHz_15kHz) / sizeof(InputSignal_f32_1kHz_15kHz[0]);
    double mean = signal_calc_mean(InputSignal_f32_1kHz_15kHz, len);
    double variance = signal_calc_variance(InputSignal_f32_1kHz_15kHz, len, mean);
    double std = signal_calc_std(variance);

    printf("Mean = %lf\n", mean);
    printf("Variance = %lf\n", variance);
    printf("Standard Deviation = %lf\n", std);
}

double signal_calc_mean(double *signal, size_t len)
{
    double accumulator = 0.0;

    for (size_t i = 0; i < len; i++)
    {
        accumulator += signal[i];
    }

    return accumulator / (double)len;
}

double signal_calc_variance(double *signal, size_t len, double signal_mean)
{
    assert(len >= 2 && "Variance needs to divide by len - 1 for reasons...");

    double variance = 0.0;

    for (size_t i = 0; i < len; i++)
    {
        variance += pow(signal[i] - signal_mean, 2);
    }

    return variance / (len - 1);
}

double signal_calc_std(double variance)
{
    return sqrt(variance);
}