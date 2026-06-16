#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
FINDCMD="$REPO_ROOT/bin/findcmp"
RESULTS_DIR="$REPO_ROOT/results"

if [ ! -d "$REPO_ROOT/testing/wide_shallow" ]; then
    echo "Dataset not found. Running setup..."
    make -C "$REPO_ROOT" setup
fi

mkdir -p "$RESULTS_DIR"

echo "=========================================="
echo "  Running All Experiments"
echo "=========================================="

run_experiment() {
    local scenario="$1"
    local root="$2"
    local csv="$RESULTS_DIR/${scenario}.csv"

    echo ""
    echo "--- Scenario: $scenario ---"
    "$FINDCMD" --algo both \
               --root "$root" \
               --target target.txt \
               --repeat 10 \
               --out "$csv"
    echo "  -> CSV:  $csv"
}

run_experiment "wide_shallow"  "$REPO_ROOT/testing/wide_shallow"
run_experiment "deep_narrow"   "$REPO_ROOT/testing/deep_narrow"
run_experiment "mixed_project" "$REPO_ROOT/testing/mixed_project"
run_experiment "no_target"     "$REPO_ROOT/testing/no_target"

echo ""
echo "=== Combining all results into all_results.csv ==="
ALL_CSV="$RESULTS_DIR/all_results.csv"
head -1 "$RESULTS_DIR/wide_shallow.csv" > "$ALL_CSV"
for f in "$RESULTS_DIR"/wide_shallow.csv \
         "$RESULTS_DIR"/deep_narrow.csv \
         "$RESULTS_DIR"/mixed_project.csv \
         "$RESULTS_DIR"/no_target.csv; do
    tail -n +2 "$f" >> "$ALL_CSV"
done
echo "Combined CSV: $ALL_CSV"

echo ""
echo "=========================================="
echo "  Experiments Complete"
echo "  Results saved to: $RESULTS_DIR/"
echo "=========================================="
