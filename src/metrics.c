#include "metrics.h"
#include <string.h>

void metrics_init(SearchMetrics *m, const char *algo, const char *root,
                  const char *target)
{
    memset(m, 0, sizeof(*m));
    strncpy(m->algorithm, algo, sizeof(m->algorithm) - 1);
    m->algorithm[sizeof(m->algorithm) - 1] = '\0';
    strncpy(m->root_path, root, sizeof(m->root_path) - 1);
    m->root_path[sizeof(m->root_path) - 1] = '\0';
    strncpy(m->target_name, target, sizeof(m->target_name) - 1);
    m->target_name[sizeof(m->target_name) - 1] = '\0';
    m->found = 0;
    m->found_path[0] = '\0';
    m->visited_dirs = 0;
    m->visited_files = 0;
    m->visited_entries = 0;
    m->skipped_dirs = 0;
    m->error_count = 0;
    m->max_frontier_size = 0;
    m->max_depth_seen = 0;
    m->elapsed_ms = 0.0;
}
