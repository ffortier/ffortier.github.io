#include <math.h>
#include <assert.h>

#include "signal_stats.h"

double signal_stats_mean(const double *signal, size_t len)
{
    double accumulator = 0.0;

    for (size_t i = 0; i < len; i++)
    {
        accumulator += signal[i];
    }

    return accumulator / (double)len;
}

double signal_stats_variance(const double *signal, size_t len, double signal_mean)
{
    assert(len >= 2 && "Variance needs to divide by len - 1 for reasons...");

    double variance = 0.0;

    for (size_t i = 0; i < len; i++)
    {
        variance += pow(signal[i] - signal_mean, 2);
    }

    return variance / (len - 1);
}

double signal_stats_std(double variance)
{
    return sqrt(variance);
}