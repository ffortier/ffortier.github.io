#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "io.h"
#include "see/da.h"
#include "see/cli.h"

void test_signal(const char *file_path, size_t expected_sample_count)
{
    SignalBuffer signal = signal_read_data(file_path);

    fprintf(stderr, "%s: sample count %zu, expected %zu\n", file_path, signal.count, expected_sample_count);

    assert(signal.count == expected_sample_count && "Expected the correct number samples in the signal");

    bool all_good = true;

    for (size_t i = 0; i < expected_sample_count; i++)
    {
        all_good &= signal.items[i] == (double)i;

        if (!all_good)
        {
            fprintf(stderr, "%s: wrong sample at index %zu, was %lf\n", file_path, i, signal.items[i]);
            break;
        }
    }

    assert(all_good && "Expected signal to have the correct values");

    see_da_free(signal);
}

int main(int argc, char **argv)
{
    const char *program = shiftarg(&argc, &argv);
    const char *small_signal_path = shiftarg(&argc, &argv);
    const char *big_signal_path = shiftarg(&argc, &argv);

    (void)program;

    fprintf(stderr, "small at %s\n", small_signal_path);

    test_signal(small_signal_path, 50);

    fprintf(stderr, "big at %s\n", small_signal_path);
    test_signal(big_signal_path, 10000);

    return 0;
}