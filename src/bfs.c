#define _POSIX_C_SOURCE 200809L
#include "bfs.h"
#include "fs_utils.h"
#include "path_queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

SearchMetrics run_bfs(const char *root_path, const char *target_name, int verbose)
{
    SearchMetrics m;
    metrics_init(&m, "BFS", root_path, target_name);

    PathQueue q;
    if (queue_init(&q) != 0) {
        fprintf(stderr, "BFS: failed to initialize queue\n");
        return m;
    }

    if (queue_push(&q, root_path) != 0) {
        fprintf(stderr, "BFS: failed to enqueue root\n");
        queue_destroy(&q);
        return m;
    }

    struct timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    while (queue_size(&q) > 0 && !m.found) {
        char *current = queue_pop(&q);

        unsigned long depth = compute_relative_depth(root_path, current);
        if (depth > m.max_depth_seen) m.max_depth_seen = depth;

        m.visited_dirs++;

        char **entries = NULL;
        size_t entry_count = 0;

        if (verbose) fprintf(stderr, "[BFS] visiting: %s\n", current);

        int read_rc = read_sorted_directory_entries(current, &entries, &entry_count);
        if (read_rc != 0) {
            if (verbose) fprintf(stderr, "[BFS] cannot open dir: %s (skipped)\n", current);
            m.skipped_dirs++;
            free(current);
            continue;
        }

        for (size_t i = 0; i < entry_count && !m.found; i++) {
            m.visited_entries++;

            char *full_path = join_path(current, entries[i]);
            if (!full_path) {
                m.error_count++;
                continue;
            }

            int type = get_entry_type_no_follow(full_path);
            if (type == ENTRY_TYPE_ERROR) {
                m.error_count++;
                free(full_path);
                continue;
            }

            if (type == ENTRY_TYPE_REGULAR) {
                m.visited_files++;
                if (strcmp(entries[i], target_name) == 0) {
                    m.found = 1;
                    strncpy(m.found_path, full_path, sizeof(m.found_path) - 1);
                    m.found_path[sizeof(m.found_path) - 1] = '\0';
                    free(full_path);
                    break;
                }
            } else if (type == ENTRY_TYPE_DIR) {
                if (verbose) fprintf(stderr, "[BFS] enqueue dir: %s\n", full_path);
                if (queue_push(&q, full_path) != 0) {
                    m.error_count++;
                } else {
                    size_t sz = queue_size(&q);
                    if (sz > m.max_frontier_size) m.max_frontier_size = sz;
                }
            }

            free(full_path);
        }

        free_string_array(entries, entry_count);
        free(current);
    }

    clock_gettime(CLOCK_MONOTONIC, &t_end);

    m.elapsed_ms = (t_end.tv_sec - t_start.tv_sec) * 1000.0 +
                   (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;

    queue_destroy(&q);

    return m;
}
