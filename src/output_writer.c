#define _POSIX_C_SOURCE 200809L
#include "output_writer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

static int file_exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0);
}

int append_metrics_csv(const char *csv_path, const SearchMetrics *metrics,
                       int run_id, int write_header_if_new)
{
    FILE *f;
    int need_header = write_header_if_new && !file_exists(csv_path);

    f = fopen(csv_path, "a");
    if (!f) {
        fprintf(stderr, "Cannot open CSV for writing: %s\n", csv_path);
        return -1;
    }

    if (need_header) {
        fprintf(f, "run_id,algorithm,root_path,target_name,found,found_path,"
                   "visited_dirs,visited_files,visited_entries,skipped_dirs,"
                   "error_count,max_frontier_size,max_depth_seen,elapsed_ms\n");
    }

    fprintf(f, "%d,%s,%s,%s,%d,%s,"
               "%lu,%lu,%lu,%lu,%lu,%lu,%lu,%.6f\n",
            run_id,
            metrics->algorithm,
            metrics->root_path,
            metrics->target_name,
            metrics->found,
            metrics->found ? metrics->found_path : "not_found",
            metrics->visited_dirs,
            metrics->visited_files,
            metrics->visited_entries,
            metrics->skipped_dirs,
            metrics->error_count,
            metrics->max_frontier_size,
            metrics->max_depth_seen,
            metrics->elapsed_ms);

    fclose(f);
    return 0;
}

void print_metrics_summary(const SearchMetrics *metrics, int run_id)
{
    (void)run_id;
    printf("Algorithm       : %s\n", metrics->algorithm);
    printf("Root            : %s\n", metrics->root_path);
    printf("Target          : %s\n", metrics->target_name);
    printf("Found           : %s\n", metrics->found ? "yes" : "no");
    printf("Found path      : %s\n", metrics->found ? metrics->found_path : "(not found)");
    printf("Visited dirs    : %lu\n", metrics->visited_dirs);
    printf("Visited files   : %lu\n", metrics->visited_files);
    printf("Visited entries : %lu\n", metrics->visited_entries);
    printf("Skipped dirs    : %lu\n", metrics->skipped_dirs);
    printf("Errors          : %lu\n", metrics->error_count);
    printf("Max frontier    : %lu\n", metrics->max_frontier_size);
    printf("Max depth seen  : %lu\n", metrics->max_depth_seen);
    printf("Elapsed         : %.3f ms\n", metrics->elapsed_ms);
    printf("----------------------------------------\n");
}
