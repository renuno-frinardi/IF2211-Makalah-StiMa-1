#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include <stddef.h>
#include "metrics.h"

int append_metrics_csv(const char *csv_path, const SearchMetrics *metrics,
                       int run_id, int write_header_if_new);

void print_metrics_summary(const SearchMetrics *metrics, int run_id);

#endif
