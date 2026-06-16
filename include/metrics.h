#ifndef METRICS_H
#define METRICS_H

typedef struct {
    char algorithm[16];
    char root_path[4096];
    char target_name[256];
    int found;
    char found_path[4096];

    unsigned long visited_dirs;
    unsigned long visited_files;
    unsigned long visited_entries;
    unsigned long skipped_dirs;
    unsigned long error_count;

    unsigned long max_frontier_size;
    unsigned long max_depth_seen;

    double elapsed_ms;
} SearchMetrics;

void metrics_init(SearchMetrics *m, const char *algo, const char *root, const char *target);

#endif
