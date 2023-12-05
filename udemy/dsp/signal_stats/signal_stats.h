#ifndef SIGNAL_STATS_H
#define SIGNAL_STATS_H

#include <stdlib.h>

double signal_stats_mean(const double *signal, size_t len);
double signal_stats_variance(const double *signal, size_t len, double signal_mean);
double signal_stats_std(double variance);

#endif