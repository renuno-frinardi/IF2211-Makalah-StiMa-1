#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#include "dfs.h"
#include "output_writer.h"

#define MAX_RUNS 10000

static void print_usage(const char *prog)
{
    printf("Usage: %s --algo bfs|dfs|both --root <path> --target <filename> [OPTIONS]\n", prog);
    printf("\nOptions:\n");
    printf("  --algo <algo>     Algorithm: bfs, dfs, or both\n");
    printf("  --root <path>     Root directory to search from\n");
    printf("  --target <name>   Exact filename to search for\n");
    printf("  --repeat <n>      Number of repetitions (default: 1)\n");
    printf("  --out <csv>       CSV output path (default: results/results.csv)\n");
    printf("  --verbose         Print detailed traversal logs to stderr\n");
    printf("  --help            Show this help\n");
}

static const char *safe_str(const char *s) { return s ? s : ""; }

int main(int argc, char **argv)
{
    const char *algo = NULL;
    const char *root = NULL;
    const char *target = NULL;
    const char *out_csv = "results/results.csv";
    int repeat = 1;
    int verbose = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        if (strcmp(argv[i], "--verbose") == 0) { verbose = 1; continue; }
        if (i + 1 >= argc) {
            fprintf(stderr, "Missing value for %s\n", argv[i]);
            return 1;
        }
        if (strcmp(argv[i], "--algo") == 0)   { algo = argv[++i]; continue; }
        if (strcmp(argv[i], "--root") == 0)   { root = argv[++i]; continue; }
        if (strcmp(argv[i], "--target") == 0) { target = argv[++i]; continue; }
        if (strcmp(argv[i], "--repeat") == 0) { repeat = atoi(argv[++i]); continue; }
        if (strcmp(argv[i], "--out") == 0)    { out_csv = argv[++i]; continue; }
        fprintf(stderr, "Unknown option: %s\n", argv[i]);
        return 1;
    }

    if (!algo || !root || !target) {
        fprintf(stderr, "Error: --algo, --root, and --target are required\n");
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(algo, "bfs") != 0 && strcmp(algo, "dfs") != 0 && strcmp(algo, "both") != 0) {
        fprintf(stderr, "Error: --algo must be bfs, dfs, or both\n");
        return 1;
    }

    if (repeat < 1 || repeat > MAX_RUNS) {
        fprintf(stderr, "Error: --repeat must be between 1 and %d\n", MAX_RUNS);
        return 1;
    }

    int run_both = (strcmp(algo, "both") == 0);
    int total_runs = repeat * (run_both ? 2 : 1);
    SearchMetrics *all_results = malloc(total_runs * sizeof(SearchMetrics));
    if (!all_results) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int run_idx = 0;

    for (int r = 0; r < repeat; r++) {
        int run_id = r + 1;

        if (run_both || strcmp(algo, "bfs") == 0) {
            if (verbose) fprintf(stderr, "--- BFS run %d ---\n", run_id);
            SearchMetrics m = run_bfs(root, target, verbose);
            print_metrics_summary(&m, run_id);
            printf("CSV saved       : %s\n", safe_str(out_csv));
            printf("\n");
            all_results[run_idx++] = m;
        }

        if (run_both || strcmp(algo, "dfs") == 0) {
            if (verbose) fprintf(stderr, "--- DFS run %d ---\n", run_id);
            SearchMetrics m = run_dfs(root, target, verbose);
            print_metrics_summary(&m, run_id);
            printf("CSV saved       : %s\n", safe_str(out_csv));
            printf("\n");
            all_results[run_idx++] = m;
        }
    }

    for (int i = 0; i < run_idx; i++) {
        append_metrics_csv(out_csv, &all_results[i], i + 1, i == 0);
    }

    free(all_results);
    return 0;
}
