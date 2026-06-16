#define _POSIX_C_SOURCE 200809L
#include "dfs.h"
#include "fs_utils.h"
#include "path_stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

SearchMetrics run_dfs(const char *root_path, const char *target_name, int verbose)
{
    SearchMetrics m;
    metrics_init(&m, "DFS", root_path, target_name);

    PathStack s;
    if (stack_init(&s) != 0) {
        fprintf(stderr, "DFS: failed to initialize stack\n");
        return m;
    }

    if (stack_push(&s, root_path) != 0) {
        fprintf(stderr, "DFS: failed to push root\n");
        stack_destroy(&s);
        return m;
    }

    struct timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    while (stack_size(&s) > 0 && !m.found) {
        char *current = stack_pop(&s);

        unsigned long depth = compute_relative_depth(root_path, current);
        if (depth > m.max_depth_seen) m.max_depth_seen = depth;

        m.visited_dirs++;

        char **entries = NULL;
        size_t entry_count = 0;

        if (verbose) fprintf(stderr, "[DFS] visiting: %s\n", current);

        int read_rc = read_sorted_directory_entries(current, &entries, &entry_count);
        if (read_rc != 0) {
            if (verbose) fprintf(stderr, "[DFS] cannot open dir: %s (skipped)\n", current);
            m.skipped_dirs++;
            free(current);
            continue;
        }

        for (size_t i = entry_count; i > 0 && !m.found; i--) {
            size_t idx = i - 1;
            m.visited_entries++;

            char *full_path = join_path(current, entries[idx]);
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
                if (strcmp(entries[idx], target_name) == 0) {
                    m.found = 1;
                    strncpy(m.found_path, full_path, sizeof(m.found_path) - 1);
                    m.found_path[sizeof(m.found_path) - 1] = '\0';
                    free(full_path);
                    break;
                }
            } else if (type == ENTRY_TYPE_DIR) {
                if (verbose) fprintf(stderr, "[DFS] push dir: %s\n", full_path);
                if (stack_push(&s, full_path) != 0) {
                    m.error_count++;
                } else {
                    size_t sz = stack_size(&s);
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

    stack_destroy(&s);

    return m;
}
